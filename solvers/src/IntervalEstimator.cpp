#include "IntervalEstimator.h"

double get_cpu_time1(){
    return (double)clock() / CLOCKS_PER_SEC;
}

check_constraint test_Interval( const Interval &in ,
                                const Interval & bound)
{
    if ( Inf(in) >= Inf(bound) && Sup(in) <= Sup(bound))
        return INSIDE;
    if ( Inf(in) > Sup(bound) ||  Sup(in) < Inf(bound))
        return OUTSIDE;
    return OVERLAP;
}

IntervalEstimator::IntervalEstimator( AbstractBasisFunction* bf, bool avoid_error)
{
    bf_ = bf;
    avoid_error_computation_ = avoid_error;
}

unsigned int IntervalEstimator::get_index_input(mem* m) const
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

unsigned int IntervalEstimator::get_index(mem* m) const
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

unsigned int IntervalEstimator::prepare_coeffs( const MogsInterval& out, unsigned int num_out)
{      
//     std::cout<<"prepare_coeffs ("<<num_out<<") " <<std::endl;
    num_out_ = num_out;
    out.get_dependancies_with_order(dep_,order_);
    
    depend_intermediate_.clear();
    for (int i=0;i<dep_.size();i++)
    {        
//         std::cout<<"dep : "<< *dep_[i]<<std::endl;
        if (dep_[i]->is_it_intermediate())
        {
            depend_intermediate_.push_back(dep_[i]->get_id_intermediate());
//             std::cout<<"depend on intermediate : "<< dep_[i]->get_id_intermediate() <<std::endl;
        }
    }
    depend_intermediate_.sort();
    
    nb_in_ = dep_.size();
    if(nb_in_ == 0)
    {
        std::cerr<<"ERROR in "<< __FILE__<<" at line "<<__LINE__<<std::endl;
        std::cerr<<"num_out = "<< num_out <<std::endl;
        std::cerr<<"name = "<< out.name_ <<std::endl;
        std::cerr<<"For the moment try to see constant value "<<std::endl;
        exit(0);
    }
    //coefdep_
    coefdep_.push_back(1);
    for (int i=0;i<nb_in_-1;i++)
        coefdep_.push_back( coefdep_[i] * (order_[i]+1));

    local_M_.resize(nb_in_);
    local_M_inverse_.resize(nb_in_);
    for (int j=0;j<nb_in_;j++)
        local_M_[nb_in_-j-1].resize(order_[j]+1,order_[j]+1);

    
    std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> > pmax(nb_in_);
    
    for( int i=0;i<nb_in_;i++)
    {
        Interval val = dep_[i]->value_;
        bf_->get_basis_coeff_matrix(val, order_[i],local_M_[nb_in_-i-1],local_M_inverse_[nb_in_-i-1]);
        bf_->get_time_max( local_M_[nb_in_-i-1], pmax[nb_in_-i-1]);
//         std::cout<<"order_["<<i<<"] = "<< order_[i] <<std::endl;
    }
    
    for (int i=0;i<nb_in_;i++)  if(! dep_[nb_in_-i-1]->rely_on_error())
    {
        local_M_inverse_inputs_.push_back(local_M_inverse_[i]);    
        pos_max_.push_back(pmax[i]);
    }
    
    kron_solver_inputs_ = new Kronecker(local_M_inverse_inputs_);
    kron_solver_errors_ = new Kronecker(local_M_inverse_);

    nb_control_point_inputs_ = kron_solver_inputs_->get_nb_control_point();
    kron_solver_inputs_->set_maximum_position(pos_max_);

    coefdep_inputs_.push_back(1);
    for (int i=0;i<nb_in_;i++)  if(! dep_[i]->rely_on_error())
    {
        dep_inputs_.push_back(dep_[i]);
        order_inputs_.push_back(order_[i]);
    }
    for (int i=0;i<dep_inputs_.size();i++)
        coefdep_inputs_.push_back( coefdep_inputs_[i] * (order_inputs_[i]+1));

    unsigned long long nb_coeff_ = 1;
    for (int i = 0;i<dep_.size();i++)
        nb_coeff_ *= order_[i]+1;

    std::list<unsigned int > coeff_inputs_, coeff_errors_;
    std::vector<unsigned int > mem_index_input(nb_control_point_inputs_);
    std::map<unsigned int, LazyVariable> MCT_coeff_;

    uint nb_valid_coeff = 0;
    nb_sparse_errors_ = 0;
    
    for( std::map<mem*,LazyVariable>::const_iterator it = out.dependances_.begin(); it != out.dependances_.end();it++)
    {        
        unsigned int index = get_index(it->first );        
        MCT_coeff_[index] = out.get_value(it->first);
        
        if(! MCT_coeff_[index].is_null())
        {
            if( it->first->contains_error())
            {
                nb_sparse_errors_++;
                coeff_errors_.push_back(index);
            }else
            {
                unsigned int index2 = get_index_input(it->first);
                coeff_inputs_.push_back(index2);
                mem_index_input[index2] = index;
            }
        }
    }

    coeff_errors_.sort();
    coeff_inputs_.sort();
    
    kron_solver_errors_->prepare_line_product_interval(coeff_errors_);
    
    sparse_coeff_errors_.resize(nb_sparse_errors_);
    
/*    // deal with error
    for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
    {
//         std::cout<<"on erreur("<< num_out<<") : "<< MCT_coeff_[*it] <<std::endl;
        LazyAddOutput(MCT_coeff_[*it],num_out_,nb_valid_coeff++);
    }      */    
    
//     std::cout<<"nb_control_point_inputs_ = " << nb_control_point_inputs_ <<std::endl;
//     std::cout<<"coeff_inputs_.size() = " << coeff_inputs_.size() <<std::endl;

//     std::cout<<"TAILLE "<< coeff_inputs_.size() <<std::endl;
    if (coeff_inputs_.size() == 0)
        nb_control_point_inputs_ = 0;
     // deal with input
    for (int i=0;i<nb_control_point_inputs_;i++)
    {
        LazyVariable out = 0;
        for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
        {
            double v = kron_solver_inputs_->get_value(i,*it);
            out += MCT_coeff_[mem_index_input[*it]]* v;
        }
//         std::cout<<"on ajoute("<< num_out<<") : "<< out <<std::endl; 
        LazyAddOutput(out,num_out_,nb_valid_coeff++);
    }
    
//     std::cout<<"avant error nb_valid_coeff = " << nb_valid_coeff <<std::endl;
    // deal with error
    for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
    {
//         std::cout<<"on erreur("<< num_out<<") : "<< MCT_coeff_[*it] <<std::endl;
        LazyAddOutput(MCT_coeff_[*it],num_out_,nb_valid_coeff++);
    }          

//     std::cout<<"apres error nb_valid_coeff = " << nb_valid_coeff <<std::endl;
    return nb_valid_coeff;
}

Interval IntervalEstimator::update_from_inputs( )
{
    unsigned int cpt = 0;  
    
    Interval out = 0.0;    
    
    if (nb_control_point_inputs_ != 0)
    {
        out = LazyUpdateOutput(num_out_,cpt++);
    }
    
    for (int i=1;i<nb_control_point_inputs_;i++)
    {
        Interval value = LazyUpdateOutput(num_out_,cpt++);
        out = Hull(out,value);
    }

    for (unsigned int i=0;i<nb_sparse_errors_;i++)
    {
        sparse_coeff_errors_(i) = LazyUpdateOutput(num_out_,cpt++);
    }

    Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);
//     std::cout<<"out = "<< out <<" error = "<< error <<std::endl;
    return out + error;
}

// check_constraint IntervalEstimator::update_from_inputs( Interval& out, Interval& bound)
check_constraint IntervalEstimator::update_from_inputs( Result& res, Interval& bound, uint index_ctr)
{  
//     std::cout<<"Check "<< index_ctr <<std::endl;
    Interval& out = res.out[index_ctr];
    double MAX =  std::numeric_limits<double>::max();
    double MIN = -std::numeric_limits<double>::max();

    double ss = Sup(bound);
    double ii = Inf(bound);
    unsigned int cpt = 0;
    bool inf_inside,sup_inside, both_side;
    
    Interval Iv = LazyUpdateOutput(num_out_,cpt++);
//     std::cout<<"value(0) = "<< Iv <<std::endl;
    for (int i=1;i<nb_control_point_inputs_;i++)
    {
        Interval value = LazyUpdateOutput(num_out_,cpt++);
//         std::cout<<"value("<<i<<") = "<< value <<std::endl;
        if (i==0)
            Iv = value;
        else
            Iv = Hull(Iv,value);
        
        double iv = Inf(Iv);
        double sv = Sup(Iv);

        
        inf_inside = (iv >= ii && iv <= ss);
        sup_inside = (sv >= ii && sv <= ss);       
        both_side = (iv <= ii && sv >= ss);
        
       
        if ( inf_inside != sup_inside || both_side)
        {
            out = Iv;
//             guess_next_bissection( i , res , before_in );
//             std::cout<<"OVERLAP at "<< i <<" / "<< nb_control_point_inputs_<<std::endl;
//             std::cout<<"out = "<< out <<" bound = "<< bound <<" before_in  = "<< before_in <<std::endl;        
//             std::cout<<"on overlap avec info de bissection" <<std::endl;
//             std::cout<<"OVERLAP inside = "<< inf_inside<< "  outside = "<< sup_inside <<" both side = "<< both_side <<std::endl;
            return OVERLAP;            
        }
    }
    
    Interval error;
    if ( Diam( res.error[num_out_]) >= OFFSET_ERROR_TH)
    {
        // deal with error   
        for (unsigned int i=0;i<nb_sparse_errors_;i++)
            sparse_coeff_errors_(i) = LazyUpdateOutput(num_out_,cpt++);
        
        error = kron_solver_errors_->line_product(sparse_coeff_errors_);
        res.error[num_out_] = error;
    }    

//     std::cout<<"error = "<< res.error[num_out_] <<std::endl;
    out = Iv + res.error[num_out_];
//     std::cout<<"out final = "<< out <<std::endl;
    if (Intersection(out,bound))
    {
        if ( Inf(out) >= Inf(bound) &&  Sup(out) <= Sup(bound) )
        {
//             std::cout<<"INSIDE FINAL"<<std::endl;
            return INSIDE;
        }
        else 
        {
//             std::cout<<"OVERLAP FINAL"<<std::endl;
            return OVERLAP;
        }
    }
//     std::cout<<"OUTSIDE FINAL"<<std::endl;
    return OUTSIDE;
}
