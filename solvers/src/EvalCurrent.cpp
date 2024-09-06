#include "EvalCurrent.h"

EvalCurrent::EvalCurrent(   AbstractCSP* pb,
                            AbstractBasisFunction* bf
)
{
    bounds_input_ = pb->get_input();
    bounds_ = pb->get_bound();
    nb_fun_ = pb->get_nb_out();
    nb_var_ = pb->get_nb_in();
    
    input_Interval.resize(nb_var_);
    output_Interval.resize(nb_fun_+ pb->get_criteria());

    for (unsigned int i=0;i<nb_var_;i++)
    {
        input_Interval[i].init( bounds_input_[i], "in_"+std::to_string(i));
    }
    pb->prepare_non_linear(input_Interval);
    pb->function(input_Interval,output_Interval);
    nb_intermediate_ = MogsInterval::get_nb_intermediate();

    MogsInterval::get_intermediate_to_compute(Intermediate_to_compute);
    MogsInterval::get_intermediate_to_update(Intermediate_to_update);
    Intermediate_to_update.resize(nb_intermediate_);
    
    infos_intermediate_update.resize(nb_intermediate_);
    
    std::cout<<"nb_intermediate_ = "<< nb_intermediate_ <<std::endl;
    for (int i=0;i<nb_intermediate_;i++)
    {
        infos_intermediate_update[i] = new IntervalEstimator( bf);        
        infos_intermediate_update[i]->prepare_coeffs(Intermediate_to_compute[i], i);
    }
    
    intermediate_updated_.resize(nb_intermediate_);    
    
    
    infos.resize(nb_fun_);
    for (int i=0;i<nb_fun_;i++)
    {
        infos[i] = new IntervalEstimator(bf);
    }

    if(solve_optim_)
    {
        info_crit_ = new IntervalEstimator(bf);
    }
    
}

void EvalCurrent::compute_intermediate_for(uint num_function)
{
    for(auto const& id : intermediate_needed_[num_function])
    {
        if (!intermediate_updated_[id])
        {
            Interval v = infos_intermediate_update[id]->update_from_inputs(current_value_);
            Intermediate_to_update[id].update( v);
            intermediate_updated_[id] = true;
        }
    }
}

void EvalCurrent::evaluate(Result& value)
{
    set_current(value);
   
    for (int i=0;i<nb_fun_;i++)
    {
        compute_intermediate_for(i);
        value.out[i] = infos[i]->update_from_inputs(value);
    }
    if(solve_optim_)
    {
        compute_intermediate_for(nb_fun_);
        value.out[nb_fun_] = info_crit_->update_from_inputs(value);
    }else
    {
        std::cout<<"on ne calcule pas le dernier"<<std::endl;
    }    
}

void EvalCurrent::init()
{
    for (int i=0;i<nb_fun_;i++)
    {
        infos[i]->prepare_coeffs(output_Interval[i], nb_intermediate_+i);
    }   
    if(solve_optim_)
    {
        info_crit_->prepare_coeffs(output_Interval[nb_fun_], nb_intermediate_+ nb_fun_);
    }

    intermediate_needed_.resize(nb_fun_ + solve_optim_);
    for (int i=0;i<nb_fun_;i++)
    {
        std::list<uint> tmp = infos[i]->get_dep_intermediate();
        intermediate_needed_[i].clear();
        get_all_intermediate_dependancies(tmp, intermediate_needed_[i]);
    }
    
    if(solve_optim_)
    {
        std::list<uint> tmp = info_crit_->get_dep_intermediate();
        intermediate_needed_[nb_fun_].clear();
        get_all_intermediate_dependancies(tmp, intermediate_needed_[nb_fun_]);        
    }

}

void EvalCurrent::get_all_intermediate_dependancies(const std::list<uint>& id_to_add,
                                                            std::list<uint> & full_list)
{
    std::list<uint> tmp;    
    for (auto const& id : id_to_add) 
    {
        if ( std::find ( full_list.begin(), full_list.end(), id) == full_list.end()) // if not id not in the list
        {
            full_list.push_back(id);
            tmp = infos_intermediate_update[id]->get_dep_intermediate();
            get_all_intermediate_dependancies(tmp,full_list);
        }
    }
    full_list.sort();
}

bool EvalCurrent::process_current(Result& current_value, optim_info& info)
{
//     std::cout<<"EvalCurrent::process_current begin : "<< current_value<<std::endl;
    set_current(current_value);
//     std::cout<<"EvalCurrent::process_current step1 : "<< current_value_<<std::endl;
    check_constraint type_optim = OVERLAP;
    Interval tmp_crit = Hull(-std::numeric_limits<double>::max(),info.optim_crit_);
    if(info.find_one_feasible_)  // if one solution found check if we can found better
    {
        compute_intermediate_for(nb_fun_);            
        type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);     
//         std::cout<<"EvalCurrent::process_current step2 : "<< current_value_<<std::endl;
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
                check_constraint ctr = infos[i]->update_from_inputs(current_value_, bounds_[i],i);
                info.additionnal_score += infos[i]->get_score_from_current_control_points();                
                
                switch(ctr)    
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
            }else
            {
                info.additionnal_score += 1.0; 
            }
//             std::cout<<"EvalCurrent::process_current step3-"<<i<<" : "<< current_value_<<std::endl;
        }      
                    
        current_value = current_value_;
        // check the optimal
        switch(type)
        {
            case(OUTSIDE)   :   return false;
                                break;
            case(INSIDE)    :   
                                if(!info.find_one_feasible_)
                                {
                                    compute_intermediate_for(nb_fun_);
                                    type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);
                                    current_value = current_value_;
                                }
                                info.additionnal_score += info_crit_->get_score_from_current_control_points();
                               switch(type_optim)
                               {
                                   case(INSIDE):
                                        info.find_one_feasible_ = true;
                                        info.optim_crit_ =  Sup(current_value_.out[nb_fun_]);
                                        info.optim_ = current_value_;
                                        return true;
                                   case(OVERLAP):
                                        return true;    
                                   case(OUTSIDE):
                                        return false;
                               }
            case(OVERLAP)   :   
//                                 bissect(current_value_,current_vector_);
                                return true;
                                break;
        }
    }
    return false;
}

void EvalCurrent::set_current( const Result& value)
{
//     std::cout<<"value = "<< value <<std::endl;
    current_value_ = value;

    for (int i=0;i<nb_var_;i++)
    {
        input_Interval[i].update( current_value_.in[i]);        
    }   
    for (int i=0;i<nb_intermediate_;i++)
    {
        intermediate_updated_[i] = false;
    }         
}
