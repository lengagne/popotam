#include "BissectionIntervalSolver.h"
#include <string.h>

BissectionIntervalSolver::BissectionIntervalSolver(AbstractCSP* pb,
                                                   const PopotamOptions *options)
{
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    
    std::string bissection_type ="undefined";
    if( ! options_->get_string_value("bissection_type",bissection_type))
    {
        std::cerr<<"Error the bissection_type : "<< bissection_type<<" is not defined" <<std::endl;
        exit(1);
    }        
    
    if( bissection_type=="MinFirst")
        bissection_type_ = 0;
    else if( bissection_type=="MaxFirst")
        bissection_type_ = 1;
    else
    {
        std::cerr<<"ERROR in "<< __FILE__<<" at line "<< __LINE__<<" the bissection type "<< bissection_type <<" is not defined"<<std::endl;
        exit(12);
    }
}

BissectionIntervalSolver::~BissectionIntervalSolver()
{
    close_files();
    results.clear();
}



void BissectionIntervalSolver::evaluate(const std::vector<Interval> &in,
                                        std::vector<Interval> &out)
{
    Result tmp;
    tmp.in = in;
    out.resize(nb_fun_);
    pb_->function(tmp.in,out);
}

param_optim BissectionIntervalSolver::solve_optim(double eps)
{
    std::cout<<"BissectionIntervalSolver::solve_optim"<<std::endl;
    start_preparation_time_ = get_cpu_time();
    precision_ = eps;

    current_vector_.clear();
    bounds_ = pb_->get_bound();
    bounds_input_ = pb_->get_input();

    Result tmp(pb_->get_input(), nb_fun_, pb_->get_criteria());

    optim_info_.optim_crit_ = std::numeric_limits<double>::max();
    optim_info_.find_one_feasible_ =false;        
    cpt_iter_ = 0;
    
    if (warm_start_)
    {        
        if (! load_warm_start_filename(save_filename_,tmp))
        {
            current_vector_.push_back(tmp);  
            optim_info_.optim_ = tmp;
        }
    }    
    else
    {
        current_vector_.push_back(tmp);            
        optim_info_.optim_ = tmp;
    }       

    std::cout<<"save_filename = "<< save_filename_<<std::endl;
    
    bool test;
    nb_valid_box_=0;
    nb_maybe_box_=0;
    
    
    start_computation_time_ = get_cpu_time();
    std::cout<<"preparation time : "<< start_computation_time_ - start_preparation_time_ <<" seconds."<<std::endl;
    
    switch(bissection_type_)
    {
        case(0):    std::cout<<"Bissection : MinFirst"<<std::endl;  break;
        case(1):    std::cout<<"Bissection : MaxFirst"<<std::endl;  break;
        default :   std::cerr<<"Bissection type not defined "<<std::endl;   std::exit(63);  break;
    }    
    
    
    if(current_vector_.size() == 0)
    {
        std::cout<<"We already load optim results (nothing  in the pile)"<<std::endl;
        return set_results();
    }    
    
//     std::cout<<"il y a "<< nb_fun_ <<" contraintes."<<std::endl;
    do{
        
        cpt_iter_++;
        if (cpt_iter_%save_each_iter_ == 0)
        {
//             std::cout<<cpt_iter_<<" crit ! "<< optim_info_.optim_crit_ <<std::endl;
            save_current_state(save_filename_);
            cpt_iter_ = 0;
            saved_iter_ ++;
        }                
        test = true;
        // We do not use reference because we pop_back !!
        Result  current_value = current_vector_.back();
        current_vector_.pop_back();

//        for (int i=0;i<nb_var_;i++)
//            std::cout<<"in["<<i<<"] = "<< current_value.in[i] <<std::endl;

        pb_->function(current_value.in,current_value.out);
//        std::cout<<"crit = "<< current_value.out[nb_fun_]<<std::endl;
        if(Inf(current_value.out[nb_fun_]) < optim_info_.optim_crit_)
        {
            check_constraint type = INSIDE;
            for (int i=0;i<nb_fun_;i++)  if(!current_value.ctr_ok[i] )
            {
//                std::cout<<"contraintes("<<i<<") = "<< current_value.out[i] <<std::endl;
                switch(test_Interval(current_value.out[i], bounds_[i]))  
                {
                    case(OUTSIDE)   :   //std::cout<<"ctr("<<i<<") OUTSIDE"<<std::endl;
                                        type = OUTSIDE;
                                        break;
                    case(INSIDE)    :   //std::cout<<"ctr("<<i<<") INSIDE"<<std::endl;
                                        current_value.ctr_ok[i] = true;
                                        break;
                    case(OVERLAP)   :   //std::cout<<"ctr("<<i<<") OVERLAP"<<std::endl;
                                        type = OVERLAP;
                                        current_value.ctr_ok[i] = false;
                                        break;
                }
//                 std::cout<<"output_Interval["<<i<<"] = "<< current_value.out[i]<<std::endl;
                if(type==OUTSIDE)
                    break;
            }
            switch(type)
            {
                case(OUTSIDE)   :   break;
                case(INSIDE)    :   // evaluate the criteria
                                    if (Sup(current_value.out[nb_fun_]) < optim_info_.optim_crit_)
                                    {
                                        optim_info_.find_one_feasible_ = true;
                                        optim_info_.optim_crit_ =  Sup(current_value.out[nb_fun_]);
                                        optim_info_.optim_ = current_value;
//                                         save_current_state(save_filename_);
                                    }
                case(OVERLAP)   :   
                                    Result low, high;
                                    bissect(current_value, current_vector_);/*
                                    
                                    if(bissect(current_value, low,high))
                                    {
                                        current.push_back(low);
                                        current.push_back(high);
                                    }                    */
                                    break;
            }
        }
        if(current_vector_.size() == 0) test = false;

    }while(test);


    current_time_ = get_cpu_time();
    return set_results();
    /*
    
    std::cout<<"Number of Bissections : "<< cpt_iter_ <<std::endl;
    std::cout<<"Number of valid boxes : "<< nb_valid_box_ <<std::endl;
    std::cout<<"Number of possible boxes : "<< nb_maybe_box_<<std::endl;
    std::cout<<"computation time (wo prep): "<< previous_time_ + current_time_ - start_computation_time_ <<" seconds."<<std::endl;
    std::cout<<"Time per iteration : "<< (previous_time_ + current_time_ - start_computation_time_)/cpt_iter_ <<" seconds."<<std::endl;
    std::cout<<"total time : "<< previous_time_ + current_time_ - start_preparation_time_ <<" seconds."<<std::endl<<std::endl;
    close_files();
    if(optim_info_.find_one_feasible_)
    {
        std::cout<<"crit = "<< optim_info_.optim_crit_ <<std::endl;
        for (int i=0;i<nb_var_;i++)
            std::cout<<"input["<<i<<"] = "<< optim.in[i]<<std::endl;
    }else
        std::cout<<"crit =  -1 \nno feasible solution found"<<std::endl;

    param_optim out;
    out.nb_bissections = cpt_iter_;
    out.nb_valid_boxes = nb_valid_box_;
    out.nb_possible_boxes = nb_maybe_box_;
    out.computation_time = previous_time_ + current_time_ - start_preparation_time_;
    out.computation_time_wo_prep = previous_time_ + current_time_ - start_computation_time_;
    out.optim = optim_info_.optim_crit_;    
    out.nb_intermediate = 0;
    out.solution_found = optim_info_.find_one_feasible_;
    return out;*/
}
