#include "BasisFunctionSolver.h"
#include <string.h>
#include <time.h>

void BasisFunctionSolver::compute_intermediate_for(uint num_function)
{
    {
        for(auto const& id : intermediate_needed_[num_function])
        {
            if (!intermediate_updated_[id])
            {
                Interval v = infos_intermediate_update[id]->update_from_inputs();
//                 std::cout<<"intermediate("<<id<<") = "<< v <<std::endl;
                Intermediate_to_update[id].update( v);
                intermediate_updated_[id] = true;
            }
        }
    }
}

void BasisFunctionSolver::get_all_intermediate_dependancies(const std::list<uint>& id_to_add,
                                                            std::list<uint> & full_list)
{
    
//     for (auto const& id : full_list) 
//         std::cout<<"full_list : "<< id <<std::endl;
    
    std::list<uint> tmp;    
    for (auto const& id : id_to_add) 
    {
//         std::cout<<"on regarde "<< id <<std::endl;
        if ( std::find ( full_list.begin(), full_list.end(), id) == full_list.end()) // if not id not in the list
        {
//             std::cout<<"on ajoute "<< id <<std::endl;
            full_list.push_back(id);
            tmp = infos_intermediate_update[id]->get_dep_intermediate();
            
//             for (auto const& jj : tmp) 
//                 std::cout<<"tmp : "<< jj <<std::endl;
            
            
            get_all_intermediate_dependancies(tmp,full_list);
        }else
        {
//             std::cout<<"on ajoute pas "<< id <<std::endl;
        }
    }
    full_list.sort();
}

void BasisFunctionSolver::init(double eps)
{
    MogsIntervalInit();
    LazyReset();
    
    start_preparation_time_ = get_cpu_time();
    precision_ = eps;
    bounds_input_ = pb_->get_input();
    bounds_ = pb_->get_bound();

    bool test;
    nb_valid_box_=0;
    nb_maybe_box_=0;    
//     ignored_space_ = 0.0;
    
    input_Interval.resize(nb_var_);
    output_Interval.resize(nb_fun_+ pb_->get_criteria());

    for (unsigned int i=0;i<nb_var_;i++)
    {
        input_Interval[i].init( bounds_input_[i], "in_"+std::to_string(i));
    }
    pb_->prepare_non_linear(input_Interval);
    pb_->function(input_Interval,output_Interval);
    nb_intermediate_ = MogsInterval::get_nb_intermediate();
    
    Result tmp(pb_->get_input(), nb_fun_+nb_intermediate_, pb_->get_criteria());
    

    
    
    optim_crit_ = std::numeric_limits<double>::max();
    find_one_feasible_ =false;        
    cpt_iter_ = 0;
    if (save_and_load_)
    {        
        if (! load_save_filename(save_filename_,tmp))
        {
            current_vector_.push_back(tmp);  
            optim_ = tmp;
        }
    }
    else
    {
        current_vector_.push_back(tmp);  
        optim_ = tmp;
    }
    
    std::cout<<"save_filename = "<< save_filename_<<std::endl;
    
    MogsInterval::get_intermediate_to_compute(Intermediate_to_compute);
    MogsInterval::get_intermediate_to_update(Intermediate_to_update);
    Intermediate_to_update.resize(nb_intermediate_);
    
    infos_intermediate_update.resize(nb_intermediate_);
    
    std::cout<<"nb_intermediate_ = "<< nb_intermediate_ <<std::endl;
    for (int i=0;i<nb_intermediate_;i++)
    {
//         std::cout<<"prepare_coeffs intermediaire de " <<i <<" / "<< nb_intermediate_ <<std::endl;
        infos_intermediate_update[i] = new IntervalEstimator( bf_);        
        infos_intermediate_update[i]->prepare_coeffs(Intermediate_to_compute[i], i);
    }
    
    intermediate_updated_.resize(nb_intermediate_);
    
//     std::cout<<"on a prepare les intermediares "<<std::endl;
}

void BasisFunctionSolver::init_end()
{
    for (int i=0;i<nb_fun_;i++)
    {
//         std::cout<<"prepare_coeffs fonction " <<i <<" / "<< nb_fun_ <<std::endl;
        infos[i]->prepare_coeffs(output_Interval[i], nb_intermediate_+i);
    }   
    if(solve_optim_)
    {
//         std::cout<<"prepare_coeffs criteria " <<nb_fun_ <<" / "<< nb_fun_ <<std::endl;
        info_crit_->prepare_coeffs(output_Interval[nb_fun_], nb_intermediate_+ nb_fun_);
    }
    
    intermediate_needed_.resize(nb_fun_ + solve_optim_);
    for (int i=0;i<nb_fun_;i++)
    {
//         std::cout<<"On construit les dépendances pour la fonction "<< i <<std::endl;
        std::list<uint> tmp = infos[i]->get_dep_intermediate();
        intermediate_needed_[i].clear();
        get_all_intermediate_dependancies(tmp, intermediate_needed_[i]);
        
//         for (auto& d : intermediate_needed_[i])
//         {
//             std::cout<<"function "<< i<<" depend de "<< d <<std::endl;
//         }
    }
    
    if(solve_optim_)
    {
        std::list<uint> tmp = info_crit_->get_dep_intermediate();
        intermediate_needed_[nb_fun_] = tmp;        
        get_all_intermediate_dependancies(tmp, intermediate_needed_[nb_fun_]);        
    }
       
    double before_compil = get_cpu_time() ;
    
    LazyPrepare();
    current_time_ = get_cpu_time();
    preparation_duration_ = current_time_ - start_preparation_time_;
    std::cout<<"preparation time : "<< preparation_duration_ <<" seconds."<<std::endl;
    start_computation_time_  = get_cpu_time();

    init_done = true;
}
void BasisFunctionSolver::set_next()
{    
    current_value_ = current_vector_.back();
    current_vector_.pop_back();         
//     std::cout<<"input_Interval.size() = "<< input_Interval.size()<<std::endl;
//     std::cout<<"current_value_.in.size() = "<< current_value_.in.size()<<std::endl;
    for (int i=0;i<nb_var_;i++)
    {
//         std::cout<<"current_value_["<<i<<"] = "<< current_value_.in[i]<<std::endl;
        input_Interval[i].update( current_value_.in[i]);        
    }   
//     std::cout<<"current_value_ = "<< current_value_<<std::endl;
    for (int i=0;i<nb_intermediate_;i++)
    {
        intermediate_updated_[i] = false;
//         #ifndef TEST_ENABLED
//         {
//             Interval v = infos_intermediate_update[i]->update_from_inputs();
//             Intermediate_to_update[i].update( v);
// //             std::cout<<"intermediate("<<i<<") = "<< v <<std::endl;
//         }
//         #endif
    }         
}



param_optim BasisFunctionSolver::solve_optim(double eps)
{
    solve_optim_ = true;
    init(eps);
    
    
    bool test;
    
    switch(bissection_type_)
    {
        case(0):    std::cout<<"Bissection : MinFirst"<<std::endl;  break;
        case(1):    std::cout<<"Bissection : MaxFirst"<<std::endl;  break;
//         case(2):    std::cout<<"Bissection : Smart"<<std::endl;  break;
        default :   std::cerr<<"Bissection type not defined "<<std::endl;   std::exit(63);  break;
    }    
    
    std::vector<double> bissect_weight(nb_var_);
    
    if(current_vector_.size() == 0)
    {
        std::cout<<"We may already load optim results (nothing in the pile)"<<std::endl;
        return set_results();
    }
    
    
//     uint max_iter = 1e3;
    do{
//         std::cout<<"*****************************************"<<std::endl;
        cpt_iter_++;
        if (cpt_iter_%save_each_iter_ == 0)
        {
//             std::cout<<cpt_iter_<<" crit ! "<< optim_crit_ <<std::endl;
            save_current_state(save_filename_);   
            cpt_iter_ = 0;
            saved_iter_ ++;            
        }
        
        test = true;
        set_next();        
        check_constraint type_optim = OVERLAP;
        Interval tmp_crit = Hull(-std::numeric_limits<double>::max(),optim_crit_);
        if(find_one_feasible_)  // if one solution found check if we can found better
        {
//             type_optim = info_crit_->update_from_inputs(tmp_crit,tout);
//             std::cout<<"dealing with optim before"<< std::endl;
            compute_intermediate_for(nb_fun_);            
            type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);   
//             current_value_.info_defined = false;
        }

        if( type_optim != OUTSIDE)
        {
            // check the constraint
            check_constraint type = INSIDE;
            for (int i=0;i<nb_fun_;i++)  
            {
                if(!current_value_.ctr_ok[i] )
                {
                    compute_intermediate_for(i);
                    switch(infos[i]->update_from_inputs(current_value_, bounds_[i],i))    
                    {
                        case(OUTSIDE)   :   //if (print_) std::cout<<" ctr("<< i<<") =  OUTSIDE"<<std::endl;
//                                             std::cout<<" ctr("<< i<<") =  OUTSIDE"<<std::endl;
                                            type = OUTSIDE;
                                            break;
                        case(INSIDE)    :   //if (print_) std::cout<<" ctr("<< i<<") =  INSIDE"<<std::endl;
//                                             std::cout<<" ctr("<< i<<") =  INSIDE"<<std::endl;
                                            current_value_.ctr_ok[i] = true;
                                            break;
                        case(OVERLAP)   :   //if (print_) std::cout<<" ctr("<< i<<") =  OVERLAP"<<std::endl;
//                                             std::cout<<" ctr("<< i<<") =  OVERLAP"<<std::endl;
                                            type = OVERLAP;
                                            current_value_.ctr_ok[i] = false;
                                            break;
                    }
                    if(type==OUTSIDE)
//                     if(type==OUTSIDE || type ==OVERLAP)
                        break;
                }
            }
            update_input();               
                        
            // check the optimal
            switch(type)
            {
                case(OUTSIDE)   :   break;
                case(INSIDE)    :   
                                    if(!find_one_feasible_)
                                    {
                                        compute_intermediate_for(nb_fun_);
//                                         std::cout<<"dealing with optim after"<< std::endl;
                                        type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);
                                    }
                                    
//                                     std::cout<<"We found one feasible : "<< current_value_.out[nb_fun_] <<std::endl;
                                    if (type_optim == INSIDE)
                                    {
                                    
//                                         std::cout<<"  tmp = "<< tmp_crit <<std::endl;
                                        find_one_feasible_ = true;
//                                         optim_crit_ =  Sup(tmp_crit);
                                        optim_crit_ =  Sup(current_value_.out[nb_fun_]);
                                        
//                                         std::cout<<"optim_crit_ = "<<current_value_.out[nb_fun_]<<std::endl<<std::endl;                                        
//                                         std::cout<<"current_value_ = "<<current_value_<<std::endl<<std::endl;
                                        optim_ = current_value_;
//                                         Result result1, result2;
                                        bissect(current_value_,current_vector_);

                                    }else  if (type_optim == OVERLAP)//if (Inf(tmp_crit) < optim_crit_)
                                    {
                                        bissect(current_value_,current_vector_);
                                    }
                                    break;
                case(OVERLAP)   :   
                                    bissect(current_value_,current_vector_);
                                    break;
            }
        }
        if(current_vector_.size() == 0) test = false;
    }while(test ); // && cpt_iter_ < max_iter_ );
//     }while(test && cpt_iter_ < max_iter );

    return set_results();
}
