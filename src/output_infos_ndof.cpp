#include "output_infos_ndof.h"


output_infos_ndof::output_infos_ndof( AbstractBasisFunction* bf)
{
    bf_ = bf;
}


// check_constraint output_infos_ndof::check_output(const Interval & check)
// {
// //    std::cout<<"contraction of constraint : "<< index_out_ <<std::endl;
//     unsigned int cpt = 0;
//     for (unsigned int i=0;i<nb_sparse_inputs_;i++)
//         sparse_coeff_inputs_(i) = code_->function(cpt++, index_out_);
//     for (unsigned int i=0;i<nb_sparse_errors_;i++)
//         sparse_coeff_errors_(i) = code_->function(cpt++, index_out_);
// 
//     Interval error = kron_solver_errors_->guess_interval(sparse_coeff_errors_);
//     Eigen::Matrix<Interval,Eigen::Dynamic,1> dual(nb_coeff_inputs_);
//     for (int i=0;i<nb_coeff_inputs_;i++)
//         dual(i) = kron_solver_inputs_->solve_line(i,sparse_coeff_inputs_);
// 
//     unsigned int nb_c = contractors_.size();
//     for (int i=0;i<nb_c;i++)
//     {
//         contractor_input& c = contractors_[i];
//         if( c.is_output)
//         {
//             double v = sparse_coeff_inputs_(c.index);
//             if(v!=0.0)
//             {
//                 Eigen::Matrix<Real,Eigen::Dynamic,1> vec(1);
//                 Eigen::Matrix<Interval,Eigen::Dynamic,1> dual_tmp = dual;
//                 vec(0) = -v;
//                 for (int j=0;j<nb_coeff_inputs_;j++)
//                 {
//                     dual_tmp(j) += specific_kron_solvers_[i]->solve_line(j,vec);
//                 }
//                 Interval input = dual_tmp(0);
//                 for (int j=1;j<nb_coeff_inputs_;j++)
//                     input = Hull(input,dual_tmp(j));
//                 input = input + error;
//                 input = -input / v;//
// 
//                 std::cout<<"check = "<< check<<" input = "<< input <<std::endl;
//                 if ( Inf(input)>=-1. &&  Sup(input)<=1.0)
//                     return INSIDE;
//                 else if ( Inf(input)>1.0 ||  Sup(input)<-1.0)
//                     return OUTSIDE;
//                 else
//                     return OVERLAP;
//             }
//         }
//     }
//     std::cerr<<"ERROR"<<std::endl;
//     return OVERLAP;
// }

/** Return true if the contraction is possible
*/
check_constraint output_infos_ndof::contract( Interval & out )
{
/*  // FIXME set the value of out
    
    unsigned int cpt = 0;
    for (unsigned int i=0;i<nb_sparse_inputs_;i++)
    {
     //   sparse_coeff_inputs_(i) = code_->function(cpt++, index_out_);
        sparse_coeff_inputs_(i) =  LazyUpdate(num_out_,cpt++);
    }
    for (unsigned int i=0;i<nb_sparse_errors_;i++)
    {
//         sparse_coeff_errors_(i) = code_->function(cpt++, index_out_);
        sparse_coeff_inputs_(i) =  LazyUpdate(num_out_,cpt++);
    }

//     Interval error = kron_solver_errors_->guess_interval(sparse_coeff_errors_);
    Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);

    Eigen::Matrix<Interval,Eigen::Dynamic,1> dual(nb_coeff_inputs_);
    
    for (int i=0;i<nb_coeff_inputs_;i++)
        dual(i) = kron_solver_inputs_->solve_line(i,sparse_coeff_inputs_);


    unsigned int nb_c = contractors_.size();
    check_constraint result = OVERLAP;
    for (int i=0;i<nb_c;i++)
    {
        contractor_input& c = contractors_[i];
        double v = sparse_coeff_inputs_(c.index);

        Interval T = Hull(-v,v);
        double sup=Sup(T);
        double inf=Inf(T);
//        if(v==0.0)
//            std::cout<<"not contracting "<< *(c.dependancy) <<" due to zero"<<std::endl;
        if(v!=0.0)
        {
            Eigen::Matrix<Real,Eigen::Dynamic,1> vec(1);
            Eigen::Matrix<Interval,Eigen::Dynamic,1> dual_tmp = dual;
            vec(0) = -v;

            dual_tmp(0) += specific_kron_solvers_[i]->solve_line(0,vec);
            Interval input = dual_tmp(0)+error;
            bool test_all = true;
            for (int j=1;j<nb_coeff_inputs_;j++)
            {
                dual_tmp(j) += specific_kron_solvers_[i]->solve_line(j,vec);
                input = Hull(input,dual_tmp(j));
                if( Inf(input)<inf && Sup(input) > sup){ test_all = false; break;}
            }

            if(test_all)
            {
                input = input + error;
                input = -input / v;//


    //            std::cout<<"Trying contracting "<< *(c.dependancy)<<" with "<< input <<std::endl;

                if(c.dependancy->contract(input) == OUTSIDE )
                {
    //                std::cout<<"OUTSIDE contracting "<< *(c.dependancy)<<" with "<< input <<std::endl;
                    return OUTSIDE;
                }
                if( c.is_output)    // Be Carefull we assume output is the last value !!
                {
    //                std::cout<<"The output value = "<< input <<std::endl;
                    // test if the computed output is the check limits
    //                if ( Inf(input) >= Inf(check) && Sup(input)<= Sup(check))
                    if ( Inf(input) >= -1.0 && Sup(input)<= 1.0)   // due to ref
                    {
                        result= INSIDE;
                    }
                }
            }
        }
    }
    return result;*/
}


unsigned int output_infos_ndof::get_index(mem* m) const
{
    unsigned int index = 0;
    unsigned int cpt = 1;
    m->compute_recurence();
    for (int i = 0;i<dep_.size();i++)
    {
        std::map<MogsInterval*, unsigned int>::const_iterator it = m->recurence.find(dep_[i]);
        if(it != m->recurence.end())
            index += (it->second ) * coefdep_[i];
    }
    return index;
}

unsigned int output_infos_ndof::get_index_input(mem* m) const
{
    unsigned int index = 0;
    unsigned int cpt = 1;
    m->compute_recurence();
    for (int i = 0;i<dep_inputs_.size();i++)
    {
        std::map<MogsInterval*, unsigned int>::const_iterator it = m->recurence.find(dep_inputs_[i]);
        if(it != m->recurence.end())
            index += (it->second ) * coefdep_inputs_[i];
    }
    return index;
}


void output_infos_ndof::prepare_coeffs(MogsInterval& out,
                                       MogsInterval& out_value,
                                       unsigned int num_out)
{
// //    std::cout<<"out = "<< out  <<std::endl;
// //    std::cout<<"out_value = "<< out_value  <<std::endl;
//     num_out_ = num_out;
//     local_dual_ = out.dual(out_value);
//     local_dual_.get_dependancies_with_order(dep_,order_);
//     nb_in_ = dep_.size();
// //    std::cout<<"local_dual_ = "<< local_dual_  <<std::endl;
// 
//     //coefdep_
//     coefdep_.push_back(1);
//     for (int i=0;i<nb_in_-1;i++)
//         coefdep_.push_back( coefdep_[i] * (order_[i]+1));
// 
//     local_M_.resize(nb_in_);
//     local_M_inverse_.resize(nb_in_);
//     for (int j=0;j<nb_in_;j++)
//         local_M_[nb_in_-j-1].resize(order_[j]+1,order_[j]+1);
// 
//     for( int i=0;i<nb_in_;i++)
//     {
//         Interval val = dep_[i]->value_;
//         bf_->get_basis_coeff_matrix(val, order_[i],local_M_[nb_in_-i-1],local_M_inverse_[nb_in_-i-1]);
//     }
// 
// 
//     coefdep_inputs_.push_back(1);
//     for (int i=0;i<nb_in_;i++)  if(! dep_[i]->rely_on_error())
//     {
//         dep_inputs_.push_back(dep_[i]);
//         order_inputs_.push_back(order_[i]);
//     }
//     for (int i=0;i<dep_inputs_.size();i++)
//         coefdep_inputs_.push_back( coefdep_inputs_[i] * (order_inputs_[i]+1));
// 
// 
//     for (int i=0;i<nb_in_;i++)  if(! dep_[nb_in_-i-1]->rely_on_error())
//         local_M_inverse_inputs_.push_back(local_M_inverse_[i]);
// 
//     nb_coeff_ = 1;
//     for (int i = 0;i<dep_.size();i++)
//         nb_coeff_ *= order_[i]+1;
// 
//     nb_coeff_inputs_ = 1;
//     for (int i = 0;i<dep_inputs_.size();i++)
//         nb_coeff_inputs_ *= order_inputs_[i]+1;
// 
//     std::map<unsigned int, LazyVariable> MCT_coeff_;
//     std::list<unsigned int > coeff_inputs_, coeff_errors_;
//     std::vector<unsigned int > mem_index_input(nb_coeff_inputs_);
//     std::vector<unsigned int> error_index, input_index;
// 
//     nb_valid_coeff_ = 0;
//     nb_sparse_inputs_ = 0;
//     nb_sparse_errors_ = 0;
// 
//     for( std::map<mem*,LazyVariable>::const_iterator it = local_dual_.dependances_.begin(); it != local_dual_.dependances_.end();it++)
//     {
//         unsigned int index = get_index(it->first );
//         MCT_coeff_[index] = local_dual_.get_value(it->first);
//         if(!MCT_coeff_[index].is_null())
//         {
//             if( it->first->contains_error())
//             {
//                 nb_sparse_errors_++;
//                 coeff_errors_.push_back(index);
//             }else
//             {
//                 unsigned int index2 = get_index_input(it->first);
//                 nb_sparse_inputs_++;
//                 coeff_inputs_.push_back(index2);
//                 mem_index_input[index2] = index;
//             }
//         }
//     }
// 
//     coeff_errors_.sort();
//     coeff_inputs_.sort();
// 
//     // deal with erro
//     for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
//     {
//         LazyAddOutput( MCT_coeff_[*it], num_out,nb_valid_coeff_++);
// //         MCT_coeff_[*it].set_as_output(nb_valid_coeff_++,index_out_);
//     }
// 
// 
//     // prepare the result
//     for (int i=0;i<nb_control_point_inputs_;i++)
//     {
//         LazyVariable out = 0;
//         for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
//         {
//             double v = kron_solver_inputs_->get_value(i,*it);
//             out += MCT_coeff_[mem_index_input[*it]]* v;
//         }
//         LazyAddOutput(out,num_out_,nb_valid_coeff_++);
//     }
//     /*    
//     std::map<unsigned int, unsigned int> coeff_index_contract;
//     for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
//     {
//         coeff_index_contract[*it] = nb_valid_coeff_;
//         LazyAddOutput( MCT_coeff_[mem_index_input[*it]], num_out,nb_valid_coeff_++);
// //         MCT_coeff_[mem_index_input[*it]].set_as_output(nb_valid_coeff_++,index_out_);
//     }*/
//     
//     
//     
// 
// 
//     sparse_coeff_inputs_.resize(nb_sparse_inputs_);
//     sparse_coeff_errors_.resize(nb_sparse_errors_);
// 
//     kron_solver_inputs_ = new Kronecker(local_M_inverse_inputs_);
//     kron_solver_errors_ = new Kronecker(local_M_inverse_);
//     kron_solver_errors_->prepare_line_product_interval(coeff_errors_);
//     
//     nb_control_point_inputs_ = kron_solver_inputs_->get_nb_control_point();
// 
//     sparse_coeff_errors_.resize(nb_sparse_errors_);
//     
//     unsigned int nb_dep = local_dual_.dependances_.size();
//     for( std::map<mem*,LazyVariable>::const_iterator it = local_dual_.dependances_.begin(); it != local_dual_.dependances_.end();it++)
//     {
//         const mem& m = *(it->first);
//         if(/*m.is_it_output() &&*/ !m.contains_error()&& m.is_contractable(contraction_mode_) )
//         {
// //            std::cout<< "we will contract "<< m<< std::endl;
//             std::vector<unsigned int> contractor_index;
//             unsigned int index = get_index_input(it->first);
//             std::list<unsigned int> tmp;
//             tmp.push_back(index);
//             contractor_index.push_back(index);
//             Kronecker* tmp_kron = new Kronecker(local_M_inverse_inputs_);
//             tmp_kron->prepare_line_product_interval(tmp);
//             specific_kron_solvers_.push_back(tmp_kron);
// 
//             contractor_input tmp_contr;
//             tmp_contr.is_output = m.is_it_output(); // remember is it is an output
// 
//             tmp_contr.dependancy = it->first;
//             tmp_contr.index = coeff_index_contract[index];
//             contractors_.push_back(tmp_contr);
//         }
//     }
//     
//     // prepare the contractions
//     
    
}

//int output_infos_ndof::necessity_contraction_productexpression(const mem &tmp)
//{
//    int problem =0;
//    for(std::list<MogsInterval*>::const_iterator it = tmp.sons.begin(); it != tmp.sons.end(); it++)
//    {
//        if(Inf((*it)->value_)<=0 && Sup((*it)->value_)>=0)
//        {
//            problem=1;
//           return problem;
//        }
//   }
//    return problem;
//}
