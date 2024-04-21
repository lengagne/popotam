#include "MogsIntervalChief.h"

MogsIntervalChief::MogsIntervalChief()
{
    //ctor
    mem tmp;
//    monomials_.push_back(*double_value_);
    double_value_ = check_input(&tmp);   // allows to define a double
    intermediate_.clear();
}

MogsIntervalChief::~MogsIntervalChief()
{
    //dtor
}

MogsInterval * MogsIntervalChief::add_intermediate_pointer( const MogsInterval& input)
{
    intermediate_.push_back(input);
    MogsInterval tmp;
    tmp.is_an_intermediare = true;
    tmp.init(Hull(0,0),"Intermediare_pointeur");
    intermediate_in_.push_back(tmp);
    MogsInterval * out = & (intermediate_in_[intermediate_in_.size()-1]);
    return out;
}

MogsInterval MogsIntervalChief::add_intermediate(const MogsInterval& inter, unsigned int M)
{
    std::map<unsigned long long , MogsInterval>::const_iterator it = memory_.find(inter.id_);
    if(it != memory_.end())
    {
//        std::cout<<"return the save one"<<std::endl;
        return it->second;
    }
    
    if (inter.rely_on_error())
    {
//        std::cout<<"create with seperated error" <<std::endl;
        MogsInterval* no_error = new MogsInterval();
        MogsInterval* error = new MogsInterval();
        for(std::map<mem*,LazyVariable>::const_iterator itmem = inter.dependances_.begin(); itmem != inter.dependances_.end(); itmem++)
            if( !(itmem->first)->contains_error())
            {
                no_error->dependances_[itmem->first] = itmem->second;
            }else
            {
                error->dependances_[itmem->first] = itmem->second;
            }

        error->id_intermediate_ = intermediate_.size();
        intermediate_.push_back(*error);

//        MogsInterval* no_error_to_init = new MogsInterval();
        MogsInterval* error_to_init = new MogsInterval();


//        no_error_to_init->init(Interval(), "NOERRIntermediate_variable_"+std::to_string(intermediate_.size()));
        error_to_init->is_error();
        error_to_init->init(Interval(), "ErrIntermediateVar_"+std::to_string(error->id_intermediate_));
        error_to_init->ref_->is_an_intermediare = true;
        error_to_init->ref_->is_error();
        error_to_init->is_an_intermediare = true;
        error_to_init->is_error();
        

//        no_error_to_init->level_ = inter.level_ + 1;
//         error_to_init->level_ = inter.level_ + 1;
        error_to_init->ref_->id_intermediate_ = error->id_intermediate_;
        error_to_init->id_intermediate_ = error->id_intermediate_;
//        intermediate_.push_back(inter);
//         std::cout<<"on a ajoute en intermediate un terme d'erreur\n"<<*error_to_init<<std::endl;
        intermediate_in_.push_back(*error_to_init);
        if(no_error->guess_size() > M/2)
        {
//             std::cout<<"add_intermediate case A"<<std::endl;
            MogsInterval out = add_intermediate(*no_error)+   (*error_to_init);
            return out;
        }

//         std::cout<<"add_intermediate case B"<<std::endl;
        
//         no_error->init(Interval(), "IntermediateVar_"+std::to_string(intermediate_.size()));
//         no_error->is_an_intermediare = true;
        
        // MogsInterval sum = (*no_error_to_init) + (*error_to_init);
        MogsInterval sum = (*no_error) + (*error_to_init);
        delete no_error;
//        std::cout<<" sum "<<sum <<std::endl;
//        memory_[&inter] = sum;
        memory_[inter.id_] = sum;
//        intermediate_in_.push_back(*no_error_to_init);
//        std::cout<<"sum.guess_size() = "<< sum.guess_size()<<std::endl;
        return sum;
    }else
    {
//         std::cout<<"add_intermediate case C"<<std::endl;
        MogsInterval new_inter;
        new_inter.is_an_intermediare = true;
        new_inter.init(Interval(-2.), "Intermediate_variable_"+std::to_string(intermediate_.size()));
        new_inter.ref_->is_an_intermediare = true;
        new_inter.ref_->id_intermediate_ = intermediate_.size();
        memory_[inter.id_] = new_inter;
//         std::cout<<"on a ajoute en intermediate un terme sans erreur"<<std::endl;
        intermediate_.push_back(inter);
        new_inter.is_an_intermediare = true;
        new_inter.id_intermediate_ = intermediate_.size();
        intermediate_in_.push_back(new_inter);
        return new_inter;
    }
}


mem* MogsIntervalChief::check_input(mem* in)
{
    std::list<mem >::iterator it = std::find(monomials_.begin(), monomials_.end(), *in);
    if(it == monomials_.end())
    {
        monomials_.push_back(*in);
        return & (*std::find(monomials_.begin(), monomials_.end(), *in));
    }
    else
    {
        delete in;
        return &(*it);
    }
}


mem* MogsIntervalChief::get_mem(const mem& in)
{
    std::list<mem >::iterator it = std::find(monomials_.begin(), monomials_.end(), in);
    if(it == monomials_.end())
    {
        return 0;
    }
    else
    {
        return &(*it);
    }
}

void MogsIntervalChief::reset()
{
    intermediate_.clear();
    intermediate_in_.clear();
    memory_.clear();
    monomials_.clear();
    
    mem tmp;
//    monomials_.push_back(*double_value_);
    double_value_ = check_input(&tmp);   // allows to define a double    
}
