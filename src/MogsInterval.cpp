#include "MogsInterval.h"
#include "MogsIntervalChief.h"

MogsIntervalChief interval_chief_;
unsigned long long nb_mogs_intervals_ = 0;

mem::mem(const mem& m):sons(m.sons)
{

}

mem::mem(MogsInterval* i)
{
    sons.push_back(i);
}

mem::mem(MogsInterval* i, unsigned int degree)
{
    for (int d=0;d<degree;d++)
        sons.push_back(i);
}

bool mem::can_contract() const
{
    for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
    {
        if( zero_inside_inter ((*it)->value_))
            return false;
    }
    return true;
}

bool mem::contains_error() const
{
    for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
    {
        if((*it)->is_an_error_)
            return true;
    }
    return false;

}

check_constraint mem::contract(const Interval& in)
{
    if (sons.size()==0) return OVERLAP;

//    if (sons.size()>1)
//    {
//        if (zero_inside_inter(in))
//            return OVERLAP;
//        if(can_contract())
//        {
//            if (!recurence_done)
//                compute_recurence();
//
//            return OVERLAP;
//        }else
//            return OVERLAP;
//    }
//    else
    {
        MogsInterval* inter = sons.front();
        if(! inter->intersection(in))
        {
            return OUTSIDE;
        }
        return OVERLAP;
    }
}

void mem::compute_recurence()
{
    if(!recurence_done)
    {
        for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
        {
            std::map<MogsInterval*,unsigned int>::const_iterator iti = recurence.find(*it);
            if (iti == recurence.end())
                recurence[*it] = 1;
            else
                recurence[*it] ++;
        }
        recurence_done = true;
    }
}

/// return the number of occurence
uint mem::get_nb( MogsInterval* ref) 
{
    compute_recurence();
    return recurence[ref];
}

bool mem::is_contractable(unsigned int how_to) const
{
    if (how_to == 0)
    {
        // contract only x
        if (sons.size()==1) return true;
        return false;
    }
    if (how_to == 1)
    {
        // contract only x, x*x, x*x*x ...
        if (sons.size() == 0 /*|| value == 0*/) return false;

        if (sons.size()==1) return true;

        std::list<MogsInterval*>::const_iterator first = sons.begin();
        for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
            if (*first != *it){
              return false;
            }
        return true;
    }
    if(how_to == 2)
    {
        // contract all
        if (sons.size()>0 )  return true;
        return false;
    }
}

bool mem::is_it_output() const
{
    for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
    {
        if( !(*it)->is_it_output() )
            return false;
    }
    if(sons.size() == 0)    return false;
    return true;
}

void mem::remove_one_ref( MogsInterval*ref)
{
    uint nb = recurence[ref];
    if (nb>1)
    recurence[ref]--;
    else 
    {
        recurence.erase(ref);
    }
    
    for(std::list<MogsInterval*>::const_iterator it = sons.begin();it != sons.end(); it++)
    {
        if (*it == ref)
        {
            sons.erase(it);
            break;
        }
    }
}

// FIXME on fait deux fois la même chose mais de 2 manière différentes
bool compare(const mem& m1,const mem& m2)
{
    std::list<MogsInterval*>::const_iterator it1 = m1.sons.begin();
    std::list<MogsInterval*>::const_iterator it2 = m2.sons.begin();
    for(; it1 != m1.sons.end() && it2 != m2.sons.end(); it1++, it2++)
        if ((*it1) != (*it2))
            return false;

    // pas fini en meme temps : donc pas de même taille
    if(it1 !=  m1.sons.end() || it2 !=  m2.sons.end() )
        return false;
    return true;
}


bool operator==(const mem& a, const mem&b)
{
    return compare(a,b);
}

std::ostream& operator<< (std::ostream& stream, const mem& m)
{
    if(m.sons.size()==0){
        stream<<"double";
        return stream;
    }
    for(std::list<MogsInterval*>::const_iterator it = m.sons.begin(); it != m.sons.end(); it++)
        stream<<"*"<<(*it)->name_ <<(*it)->value_;
    return stream;
}

void MogsIntervalInit()
{
    interval_chief_.reset();
}

MogsInterval::MogsInterval()
{
    id_ = nb_mogs_intervals_++;
    //ctor
    name_ ="not_defined";
    value_ = Hull(0);
    dependances_.clear();
    the_sons_.clear();
}

MogsInterval::MogsInterval(const LazyVariable & in) //:value_(in.value_)
{
    id_ = nb_mogs_intervals_++;
    middle_ = in;
    diam_ = LazyVariable(0.);

//    value_ = in.eval();
    mem* tmp = interval_chief_.get_double();
    value_ = 0;
    dependances_.clear();
    dependances_[tmp] = in;
    the_sons_.clear();
}

MogsInterval::MogsInterval(const double & d)
{
    id_ = nb_mogs_intervals_++;
    name_ ="double";
    value_ = d;
    middle_ = d;
    diam_ = 0.;
    mem* tmp = interval_chief_.get_double();
    dependances_[tmp] = d;
    the_sons_.clear();
}

MogsInterval::MogsInterval(const MogsInterval & in):value_(in.value_), name_(in.name_),
                                                    the_sons_(in.the_sons_),is_an_error_(in.is_an_error_),
                                                    is_input_(in.is_input_),
                                                    is_an_intermediare(in.is_an_intermediare),
                                                   id_intermediate_(in.id_intermediate_)
{
    id_ = nb_mogs_intervals_++;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = in.dependances_.begin(); itmem != in.dependances_.end(); itmem++)
        if (!itmem->second.is_null()) // FIXME !!
        {
            dependances_[itmem->first] = itmem->second;
        }
    if(is_input_)
    {
        ref_ = in.ref_;
        middle_ = in.middle_;
        diam_ = in.diam_;
    }
}

MogsInterval::~MogsInterval()
{
    //dtor
    dependances_.clear();
}

MogsInterval::MogsInterval(const Interval& val,
                            const std::string& name):value_(val),name_(name),is_input_(true)
{
    mem* tmp = new mem;
    tmp->sons.push_back(this);
    mem* tmp1 = interval_chief_.check_input(tmp);
    dependances_[tmp1] = LazyVariable(1.0);
    the_sons_.clear();
    
    diam_ = Diam(val);
    middle_ = Mid(val);
}

void MogsInterval::add (const LazyVariable&in, mem * m)
{
    dependances_[m]= in;
}

MogsInterval MogsInterval::add_intermediate( const MogsInterval & inter,
                                             const unsigned int M)
{
//     std::cout<<"add_intermediate inter = "<< inter <<std::endl;
    return interval_chief_.add_intermediate(inter,M);
}

mem* MogsInterval::chief_check_input(mem* in) const
{
    return interval_chief_.check_input(in);
}

check_constraint MogsInterval::contract_of_order(const Interval& in,
                                                 unsigned int degree)
{
    if (degree %2 == 0) // pair degree
    {
         Interval sqr1 = Hull(pow(Inf(in),1./degree), pow(Sup(in),1./degree));
         Interval sqr2 = -sqr1;
         Interval tot;

         if (Inf(value_) <=0 && Sup(value_)>= 0)
         {
             tot = Hull(sqr1,sqr2);
         }else  if (Sup(value_) <=0 )
         {
             tot = sqr2;
         }else  if (Inf(value_) >=0 )
         {
             tot = sqr1;
         }else
         {
             std::cerr<<"There is a big issue !!"<<std::endl;
             std::cout<<"inter = "<< tot<<std::endl;
         }
         if(!intersection(tot))
             return OUTSIDE;
    }else   // unpair degree
    {
        Interval t = Hull(pow(Inf(in),1./degree), pow(Sup(in),1./degree));
        if(!intersection(t))
            return OUTSIDE;
    }
    return OVERLAP;
}


MogsInterval MogsInterval::derivative(MogsInterval* ref) const
{
    MogsInterval out;

    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
    {       
        mem *m = new mem(*itmem->first);
        uint nb = m->get_nb(ref);
        if (nb>0)
        {            
            m->remove_one_ref(ref);
            LazyVariable tmp = itmem->second;
            out.add(nb*tmp,m);
        }
    }
    return out;
}

MogsInterval MogsInterval::dual ( const MogsInterval& I) const
{
    MogsInterval out;
    out.value_ = value_ - I.value_;

    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
        out.dependances_[itmem->first] = itmem->second;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = I.dependances_.begin(); itmem != I.dependances_.end(); itmem++)
        out.dependances_[itmem->first] -= itmem->second;
    return out;
}

void MogsInterval::get_dependancies_with_order(std::vector<MogsInterval*> &dep, std::vector<unsigned int> & order ) const
{
    dep.clear();
    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
    {
        const mem& m = *(itmem->first);
        for(std::list<MogsInterval*>::const_iterator it = m.sons.begin(); it != m.sons.end(); it++)
        {
            bool to_add = true;
            for(int k=0;k<dep.size();k++)   if(*it == dep[k])
            {
                to_add = false;
                break;
            }
            if(to_add)  dep.push_back(*it);
        }
    }
    for(int k=0;k<dep.size();k++)   order.push_back(get_maximal_order(*dep[k]));
}

void MogsInterval::get_reference( MogsInterval** ref) const
{
    std::vector<MogsInterval*> dep;
    std::vector<unsigned int> order;
    get_dependancies_with_order(dep,order);
    assert(dep.size() == 1);
    assert(order[0] == 1);
    *ref = dep[0];
}

bool MogsInterval::get_contract_from_ref(std::vector<Real>& v,
                                         Interval &i)
{
//     i =  ref_->value_ * diam_.get_double_from_vector(v)/2. +  middle_.get_double_from_vector(v);
    i =  ref_->value_ * diam_.get_value()/2. +  middle_.get_value();
    if (ref_->value_ != Hull(-1,1))
    {
        ref_->value_ = Hull(-1,1);
        return true;
    }
    return false;
}

void MogsInterval::get_intermediate(std::vector<MogsInterval*> & inters,
                                    std::vector<unsigned int > & ids,
                                    bool with_error) const
{
    std::vector<MogsInterval*> dep;
    std::vector<unsigned int>  order;
    get_dependancies_with_order(dep,order);
    for (int i=0;i<dep.size();i++)
    {
        if(dep[i]->is_it_intermediate() && (with_error|| !dep[i]->is_it_error()))
        {
            inters.push_back(dep[i]);
            ids.push_back(dep[i]->id_intermediate_);
        }
    }
}

unsigned int MogsInterval::get_nb_intermediate()
{
    return interval_chief_.get_nb_intermediate();
}

void MogsInterval::get_intermediate_to_compute(std::vector<MogsInterval> & out)
{
    interval_chief_.get_intermediate_to_compute(out);
}

void MogsInterval::get_intermediate_to_update(std::vector<MogsInterval> & out)
{
    interval_chief_.get_intermediate_to_update(out);
}

// unsigned int MogsInterval::get_max_level()
// {
//     return interval_chief_.get_max_level();
// }

unsigned int MogsInterval::get_maximal_order(const MogsInterval& var) const
{
    unsigned int order = 0;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
    {
        const mem& m = *(itmem->first);
        unsigned int tmp_cpt = 0;
        for(std::list<MogsInterval*>::const_iterator it = m.sons.begin(); it != m.sons.end(); it++)
            if( (*it)->name_ == var.name_)
                tmp_cpt ++;
        if(tmp_cpt > order)
            order = tmp_cpt;
    }
    return order;
}

MogsInterval* MogsInterval::get_son(const sons_type& type) const
{
    unsigned int nb = the_sons_.size();
    for (int i=0;i<nb;i++)
    {
        if (the_sons_[i].type == type)
        {
            return the_sons_[i].son;
        }
    }
    std::cerr<<"Error in "<< __FILE__<<" at line :"<< __LINE__<<std::endl;
    std::cerr<<"Error MogsInterval::get_son type : "<<type<<" (for non linear error) is not defined"<<std::endl;
    exit(123);
}

mem* MogsInterval::get_double_mem()const
{
    return interval_chief_.get_double();
}

mem* MogsInterval::get_mem_pointeur(const mem& m) const
{
    return interval_chief_.get_mem(m);
}

LazyVariable MogsInterval::get_value(mem* m) const
{
    std::map<mem*,LazyVariable>::const_iterator it = dependances_.find(m);
    if(it == dependances_.end())
    {
        return 0;
    }
    return it->second;
}

unsigned int MogsInterval::guess_size() const
{
    std::vector<MogsInterval*> dep;
    std::vector<unsigned int>  order;
    get_dependancies_with_order(dep,order);
    unsigned int s = 1;
    for (int i=0;i<order.size();i++)
        s*= order[i]+1;
//     std::cout<<"guess_size() : "<< s <<std::endl;
    return s;
}

void MogsInterval::init(const Interval& val,
                        const std::string& name)
{
    value_.update(val);
    mem* tmp = new mem;
    tmp->sons.push_back(this);
    mem* tmp1 = interval_chief_.check_input(tmp);
    dependances_[tmp1] = LazyVariable(1.0);
    the_sons_.clear();
    is_input_ = true;
    diam_ = LazyVariable(Diam(val),"D_"+name);  //create as input
    middle_ = LazyVariable(Mid(val),"M_"+name); //create as input
    ref_ = new MogsInterval(Hull(-1,1),"Ref_"+name);
    if(is_an_error_)
        ref_->is_error();
    *this = *ref_ * diam_/2. + middle_;
    is_input_ = true;
    name_ = name;    
}

bool MogsInterval::intersection(const Interval& in, bool contract_son)
{
    Interval out;
    bool ret = Intersection(out,value_,in);
    if(ret && value_ != out)
    {
        value_ = out;
    }
    return ret;
}

bool MogsInterval::inside(const Interval& in)
{
    return ( Inf(in)>= Inf(value_) && Sup(in) <= Sup(value_));
}

void MogsInterval::prepare_son(const sons_type& type)
{
    unsigned int nb = the_sons_.size();
    for (int i=0;i<nb;i++)
    {
        if (the_sons_[i].type == type)
            return ;
    }
    // do not found so create one
    sons new_son;
    new_son.son = new MogsInterval();
    the_sons_.push_back(new_son);
    the_sons_[nb].type = type;
    switch(type)
    {
        case(SON_COS_ERROR):
            the_sons_[nb].son->is_error();
            the_sons_[nb].son->init(Hull(-1,1),"erc_"+name_);
            break;
        case(SON_SIN_ERROR):
            the_sons_[nb].son->is_error();
            the_sons_[nb].son->init(Hull(-1,1),"ers_"+name_);
            break;
//         case(SON_SIN_COS_ERROR):
//             the_sons_[nb].son->is_error();
//             the_sons_[nb].son->init(Hull(-1,1),"ersc_"+name_);
            break;
        case(SON_SIGNOF_ERROR):
            the_sons_[nb].son->is_error();
            the_sons_[nb].son->init(Hull(-1,1),"ersign_of"+name_);
            break;
/*         case(DIVIDE_ERROR):
            the_sons_[nb].son->is_error();
            the_sons_[nb].son->init(Hull(-1,1),"erdivide"+name_);
            break;   */    
        default:
            std::cerr<<"ERROR in "<< __FILE__<<" at line "<< __LINE__<<std::endl;
            std::cerr<<"Error in MogsInterval::prepare_son : type : "<< type<< " (for non linear error) is not defined"<<std::endl;
            exit(123);
    }
}


bool MogsInterval::rely_on_error()const
{
    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
        if((itmem->first)->contains_error())
            return true;
    return false;
}

// void MogsInterval::update(const Interval & in, std::vector<Real>& v)
void MogsInterval::update(const Interval & in)
{
    value_.update(in);
    middle_ = Mid(in);
    LazyUpdateInput( middle_.creator_->id_, Mid(in));
//     LazyUpdateInput( middle_.ref_->id_, Mid(in));        
    diam_ = Diam(in);
    LazyUpdateInput( diam_.creator_->id_, Diam(in));
//     LazyUpdateInput( diam_.ref_->id_, Diam(in));
    
    ref_->value_ = Hull(-1,1);
    for (int i=0;i<the_sons_.size();i++)
    {
        switch (the_sons_[i].type)
        {
            case(SON_COS_ERROR):  
//                             std::cout<<"SON_COS_ERROR"<<std::endl;
                            the_sons_[i].son->update_error_cos(in); // ,v);
                            break;
            case(SON_SIN_ERROR):
//                             std::cout<<"SON_SIN_ERROR"<<std::endl;
                            the_sons_[i].son->update_error_sin(in); // ,v);
                            break;
//             case(SON_SIN_COS_ERROR):  
// //                             std::cout<<"SON_SIN_COS_ERROR"<<std::endl;
//                             the_sons_[i].son->update_error_sin_cos(in); // ,v);
//                             break;
            case(SON_SIGNOF_ERROR):  
//                             std::cout<<"SON_SIGNOF_ERROR"<<std::endl;
                            the_sons_[i].son->update_error_sign_of(in); // ,v);
                            break;                            
//             case(DIVIDE_ERROR):  
// //                             std::cout<<"DIVIDE_ERROR"<<std::endl;
//                             the_sons_[i].son->update_error_division(in); // ,v);
//                             break;                                                      
            default:    std::cerr<<"ERROR in "<< __FILE__ <<" at line "<<__LINE__<< std::endl;
                        exit(0);
        }
    }
}

bool MogsInterval::operator== (const double& in) const
{
    if (dependances_.size() == 0) 
    {
        if (in == 0)
            return true;
        return false;
    }
    
    if (dependances_.size() > 1) 
    {
        return false;
    }
    
    std::map<mem*,LazyVariable>::const_iterator it1 = dependances_.begin();
    if (it1->first->is_double() && it1->second == in)
    {
        return true;
    }
    return false;    
}

bool MogsInterval::operator== (const MogsInterval& I) const
{
    if (dependances_.size() != I.dependances_.size())    return false;
    std::map<mem*,LazyVariable>::const_iterator it1 = dependances_.begin();
    std::map<mem*,LazyVariable>::const_iterator it2 = I.dependances_.begin();
    for(; it1 != dependances_.end() && it2 != I.dependances_.end(); it1++, it2++)
    {
        if (it1->first != it2->first)   return false;
        if (it1->second != it2->second)   return false;
    }
    return true;
}

MogsInterval MogsInterval::operator= (const MogsInterval& in)
{
     if (this != &in)
     {
//         name_ = in.name_;
        value_ = in.value_;
        dependances_.clear();
//        dependances_ = in.dependances_;
        for(std::map<mem*,LazyVariable>::const_iterator itmem = in.dependances_.begin(); itmem != in.dependances_.end(); itmem++)
            if (!itmem->second.is_null()) // FIXME
            {
                dependances_[itmem->first] = itmem->second;
            }
        the_sons_ = in.the_sons_;
        is_an_error_ = in.is_an_error_;
        is_input_ = in.is_input_;
//         level_ = in.level_;
        is_an_intermediare = in.is_an_intermediare;
        id_ = in.id_;
        id_intermediate_ = in.id_intermediate_;
        if(is_input_)
        {
            ref_ = in.ref_;
            middle_ = in.middle_;
            diam_ = in.diam_;
        }
     }
    return *this;
}

MogsInterval MogsInterval::operator- () const
{
    MogsInterval out = *this;
    out.value_ = -value_;
    for(std::map<mem*,LazyVariable>::iterator itmem = out.dependances_.begin(); itmem != out.dependances_.end(); itmem++)
        itmem->second *= -1;
    return out;
}

MogsInterval MogsInterval::operator- (const MogsInterval& I) const
{
    MogsInterval out;
    out.value_ = value_ - I.value_;

    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
        out.dependances_[itmem->first] = itmem->second;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = I.dependances_.begin(); itmem != I.dependances_.end(); itmem++)
        out.dependances_[itmem->first] -= itmem->second;

    // std::cout<<"operator- : "<< out <<std::endl;
    if(out.guess_size() > MAXSIZE)
    {
        MogsInterval new_intermediate = add_intermediate(out);
        return new_intermediate;
    }
    return out;
}

MogsInterval MogsInterval::operator+ (const MogsInterval& I) const
{
    MogsInterval out;
    out.value_ = value_ + I.value_;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = dependances_.begin(); itmem != dependances_.end(); itmem++)
        out.dependances_[itmem->first] = itmem->second;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = I.dependances_.begin(); itmem != I.dependances_.end(); itmem++)
    {
        out.dependances_[itmem->first] += itmem->second;
    }
    // std::cout<<"operator+ : "<< out <<std::endl;
    if(out.guess_size() > MAXSIZE)
    {
        MogsInterval new_intermediate = add_intermediate(out);
        return new_intermediate;
    }
    return out;
}

void MogsInterval::operator+= (const MogsInterval& I)
{
    for(std::map<mem*,LazyVariable>::const_iterator itmem = I.dependances_.begin(); itmem != I.dependances_.end(); itmem++)
    {
        dependances_[itmem->first] += itmem->second;    
    }    
    // We must need to add this line to consider new variable !!
    id_ = nb_mogs_intervals_++;
    // std::cout<<"operator+= : "<< *this <<std::endl;
    if(guess_size() > MAXSIZE)
    {
        *this =  add_intermediate(*this);
    }
}

void MogsInterval::operator-= (const MogsInterval& I)
{
    for(std::map<mem*,LazyVariable>::const_iterator itmem = I.dependances_.begin(); itmem != I.dependances_.end(); itmem++)
        dependances_[itmem->first] -= itmem->second;
    // We must need to add this line to consider new variable !!
    id_ = nb_mogs_intervals_++;    
    // std::cout<<"operator-= : "<< *this <<std::endl;
    if(guess_size() > MAXSIZE)
    {
        *this =  add_intermediate(*this);
    }
}

MogsInterval MogsInterval::operator* (const double& d) const
{
    if (d==0)
    {
        return 0.0;        
    }
    if (d==1.0)
    {
        return *this;
    }
    
    MogsInterval out = *this;
    out.value_ *= d;
    for(std::map<mem*,LazyVariable>::iterator itmem = out.dependances_.begin(); itmem != out.dependances_.end(); itmem++)
        itmem->second *= d;

    return out;
}

MogsInterval MogsInterval::operator* (const LazyVariable& d) const
{
    MogsInterval out = *this;
    for(std::map<mem*,LazyVariable>::iterator itmem = out.dependances_.begin(); itmem != out.dependances_.end(); itmem++)
        itmem->second *= d;
    // std::cout<<"operator* : "<< out <<std::endl;
    if(out.guess_size() > MAXSIZE)
    {
        return add_intermediate(out);
    }
    return out;
}


MogsInterval MogsInterval::operator* (const MogsInterval& in) const
{
//     std::cout<<"Operator * this : "<< *this <<std::endl;
//     std::cout<<"Operator * in : "<< in <<std::endl;
    if (in == 0.0)
    {
        return 0.0;
    }
    if (in == 1.0)
    {
        return *this;    
    }
    if (*this == 0.0)
    {
        return 0.0;
    }
    if (*this == 1.0)
    {
        return in;        
    }
   // std::cout<<"operator- : "<< out <<std::endl;
    if(guess_size() > MAXSIZE && in.guess_size() > MAXSIZE)
    {
        MogsInterval i1 = add_intermediate(*this);
        MogsInterval i2 = add_intermediate(in);
        return i1*i2;
    }

    MogsInterval out;
    out.value_ = value_ * in.value_;
    if( this == &in)
    {
        if(guess_size() > MAXSIZE/2)
        {
            MogsInterval i1 = add_intermediate(*this,MAXSIZE/2);
            return i1*i1;
        }

        for(std::map<mem*,LazyVariable>::const_iterator it1 = dependances_.begin(); it1 != dependances_.end(); it1++)
        {
            const mem &  m1 = *it1->first;
            mem* tmp = new mem;
            for(std::list<MogsInterval*>::const_iterator it = m1.sons.begin(); it != m1.sons.end(); it++)
            {
                tmp->sons.push_back(*it);
                tmp->sons.push_back(*it);
            }
            tmp->sons.sort(compareMogsById);
            mem* good = interval_chief_.check_input(tmp);
            out.dependances_[good] += it1->second * it1->second;

            std::map<mem*,LazyVariable>::const_iterator it2 = it1;
            it2++;
            for(; it2 != dependances_.end(); it2++)
            {
                const mem &  m2 = *it2->first;
                mem* tmp = new mem;
                for(std::list<MogsInterval*>::const_iterator it = m1.sons.begin(); it != m1.sons.end(); it++)
                tmp->sons.push_back(*it);
                for(std::list<MogsInterval*>::const_iterator it = m2.sons.begin(); it != m2.sons.end(); it++)
                tmp->sons.push_back(*it);
                tmp->sons.sort(compareMogsById);
                mem* good = interval_chief_.check_input(tmp);
                out.dependances_[good] += it1->second * it2->second *2.0;
            }
        }

        //if(out.get_nb_value() > MAXMUL)
        if(out.guess_size() > MAXSIZE)
        {
            MogsInterval new_intermediate = add_intermediate(out);
            return new_intermediate;
        }
        return out;
    }
    unsigned int cpt = 0;
    for(std::map<mem*,LazyVariable>::const_iterator it1 = dependances_.begin(); it1 != dependances_.end(); it1++)
    {
        const mem &  m1 = *it1->first;
        for(std::map<mem*,LazyVariable>::const_iterator it2 = in.dependances_.begin(); it2 != in.dependances_.end(); it2++)
        {
            const mem &  m2 = *it2->first;
            mem* tmp = new mem;
            for(std::list<MogsInterval*>::const_iterator it = m1.sons.begin(); it != m1.sons.end(); it++)
                tmp->sons.push_back(*it);

            for(std::list<MogsInterval*>::const_iterator it = m2.sons.begin(); it != m2.sons.end(); it++)
                tmp->sons.push_back(*it);
            tmp->sons.sort(compareMogsById);
            mem* good = interval_chief_.check_input(tmp);
            out.dependances_[good] += it1->second * it2->second;
        }
    }
    if(out.guess_size() > MAXSIZE)
    {
        MogsInterval new_intermediate = add_intermediate(out);
        return new_intermediate;
    }
//     std::cout<<"Operator * out : "<< out <<std::endl;
    return out;
}

std::ostream& operator<< (std::ostream& stream, const MogsInterval& inter)
{
    stream<<"("<<inter.name_<<") = "<< inter.value_;
//     stream<<"  mid = "<< inter.middle_;
//     stream<<"  diam = "<< inter.diam_;
    stream<<std::endl;
    unsigned int cpt=1;
    for(std::map<mem*,LazyVariable>::const_iterator itmem = inter.dependances_.begin(); itmem != inter.dependances_.end(); itmem++)
        stream<<"\t M("<<cpt++<<") = " << itmem->second<<"\t type : " << *(itmem->first)<<std::endl;
    
//     stream<<"  nb sons = "<< inter.the_sons_.size();
    
    return stream;
}
/// ATTENTION ICI C'EST TOUT POURRI ON PREND LE COS ET LE SINUS QUE D'UNE ENTREE => A REVOIR
void MogsInterval::update_error_cos(const Interval  & in) //, std::vector<Real>& v )
{
    /// FIXME we assume the largest error is at the extremum
    Real m = Mid(in);
//     Real binf = approx_taylor_cos<Real,Real>(Inf(in),m);
//     Real bsup = approx_taylor_cos<Real,Real>(Sup(in),m);
// 
//     Interval approx = Hull( Hull(binf,bsup), cos(m));
//     Interval reel = cos(in);
//     value_ = Hull(Hull(Inf(reel)-Inf(approx),Sup(reel)-Sup(approx)),0);

    Real inf_error = cos (Inf(in)) - approx_taylor_cos<Real,Real>(Inf(in),m);
    Real sup_error = cos (Sup(in)) - approx_taylor_cos<Real,Real>(Sup(in),m);
    value_ = Hull(Hull(inf_error,sup_error),0);    
    
    update(value_);
}

void MogsInterval::update_error_division(const Interval & in) //, std::vector<Real>& v)
{
    value_ = Hull(1.0,1.0) /in;
}

void MogsInterval::update_error_sin(const Interval  & in) //, std::vector<Real>& v)
{
    /// FIXME we assume the largest error is at the extremum
    Real m = Mid(in);
//     Real binf = approx_taylor_sin<Real,Real>(Inf(in),m);
//     Real bsup = approx_taylor_sin<Real,Real>(Sup(in),m);
// 
//     Interval approx = Hull( Hull(binf,bsup), sin(m));
//     Interval reel = sin(in);
//     value_ = Hull(Hull(Inf(reel)-Inf(approx),Sup(reel)-Sup(approx)),0);

    Real inf_error = sin (Inf(in)) - approx_taylor_sin<Real,Real>(Inf(in),m);
    Real sup_error = sin (Sup(in)) - approx_taylor_sin<Real,Real>(Sup(in),m);
    value_ = Hull(Hull(inf_error,sup_error),0);
    
    update(value_);
}

// void MogsInterval::update_error_sin_cos(const Interval  & in) //, std::vector<Real>& v )
// {
//     /// FIXME we assume the largest error is at the extremum
// 
//     Real m = Mid(in);
//     Real binf = approx_taylor_cos<Real,Real>(Inf(in),m);
//     Real bsup = approx_taylor_cos<Real,Real>(Sup(in),m);
// 
//     Interval approx = Hull( Hull(binf,bsup), cos(m));
//     Interval reel = cos(in);
//     Interval valuecos_ = Hull(Hull(Inf(reel)-Inf(approx),Sup(reel)-Sup(approx)),0);
// 
//     binf = approx_taylor_sin<Real,Real>(Inf(in),m);
//     bsup = approx_taylor_sin<Real,Real>(Sup(in),m);
// 
//     approx = Hull( Hull(binf,bsup), sin(m));
//     reel = sin(in);
//     Interval valuesin_ = Hull(Hull(Inf(reel)-Inf(approx),Sup(reel)-Sup(approx)),0);
// 
//     value_ = Hull(valuesin_,valuecos_);
// 
//     update(value_);
// }

void MogsInterval::update_error_sign_of(const Interval & in) // , std::vector<Real>& v)
{
    if (in == 0)
        value_ = 0;
    else if (Inf(in)> 0)
        value_ = 1;
    else if (Sup(in)< 0)
        value_ = -1;
    else
        value_ = Hull(-1,1);
    update(value_);
}

void merge_dependancies(  std::vector<MogsInterval*> &dep1,
                                        std::vector<MogsInterval*> &dep2,
                                        std::vector<unsigned int> & order1,
                                        std::vector<unsigned int> & order2)
{
    uint nb1 = dep1.size();
    uint nb2 = dep2.size();
    std::vector<bool> found(nb2);
    for (int i=0;i<nb2;i++) found[i] = false;
    
    // we check
    for (int i=0;i<nb1;i++)
    {
        for (int j=0;j<nb2;j++)
        {
            if ( dep1[i] == dep2[j])
            {
                found[j] = true;
                if (order2[j] > order1[i])  order1[i]=order2[j];
            }
        }
    }
    // we add if not in dep1
    for (int j=0;j<nb2;j++) if(!found[j])
    {
        dep1.push_back(dep2[j]);
        order1.push_back(order2[j]);
    }    
}

// // WARNING calling non linear function will create new variable !!
// MogsInterval MogsInterval::operator/ (const MogsInterval& den) const
// {
//     LazyVariable N = get_mid_value();
//     LazyVariable D = den.get_mid_value();
//     
//     MogsInterval out = N/D;
//     
//     // on recupere toutes les entrées
//     std::vector<MogsInterval*> depN,depD;
//     std::vector<unsigned int> orderN,orderD;
//      
//     get_dependancies_with_order(depN,orderN);
//     den.get_dependancies_with_order(depD,orderD);
//     
//     std::cout<<"depN :  "<< depN.size() <<" dépendances "<<std::endl;
//     std::cout<<"depD :  "<< depD.size() <<" dépendances "<<std::endl;
//     
//     for (int i=0;i<depD.size();i++)
//     {
//         bool test = false;
//         for (int j=0;j<depN.size();j++)
//         {
//             if(depD[i] == depN[j])
//             {
//                 std::cout<<"depN[i] :  "<< *depN[i] <<" depN : "<< * depD[i] <<std::endl;
//                 test = true;
//                 break;
//             }
//         }
//         if (!test)
//         {
//             depN.push_back(depD[i]);
//         }
//     }
//     
//     uint nb = depN.size();
//     uint nb_approx = 1;
//     std::cout<<"On aura "<< nb <<" dépendances "<<std::endl;
//     
//     MogsInterval up,vp,NUM,DEN;
//     const MogsInterval& u=*this;
//     const MogsInterval& v = den;
//     
//     for (int i=0;i<depN.size();i++)
//     {
//         // derivee par rapport à la premiere variable
//         up = u.derivative(depN[i]);
//         vp = v.derivative(depN[i]);
//         
//         NUM = up*v - u *vp;
//         DEN = v*v;
//         N = NUM.get_mid_value();
//         D = DEN.get_mid_value();
//         out += (*depN[i]) * LazyVariable(N/D);
//     }
//     
// //     // derivee par rapport à la deuxieme variable
// //     std::cout<<"u = "<< u << std::endl;
// //     std::cout<<"depN[1] = "<< *depN[1] << std::endl;
// //     up = u.derivative(depN[1]);
// //     vp = v.derivative(depN[1]);
// //     
// //     NUM = up*v - u *vp;
// //     DEN = v*v;
// //     N = NUM.get_mid_value();
// //     D = DEN.get_mid_value();
// //     out += (*depN[1]) * LazyVariable(N/D);    
//     
// //     MogsInterval*  tmp = interval_chief_.add_intermediate_pointer(in);
// //     tmp->name_ = "Inverseof"+in.name_;
// //     tmp->prepare_son(DIVIDE_ERROR);  
// //     
// //     MogsInterval * er_div = tmp->get_son(DIVIDE_ERROR);    
// //     MogsInterval out =  approx_taylor_division<MogsInterval,LazyVariable>(*this,middle_,den,den.middle_) ; //+ *er_div;
//     return out;    
// }


// WARNING calling non linear function will create new variable !!
MogsInterval cos(const MogsInterval& in)
{
    MogsInterval * er_cos = in.get_son(SON_COS_ERROR);
    MogsInterval out = approx_taylor_cos<MogsInterval,LazyVariable>(in,in.middle_) + *er_cos; // + *er ; //
    out.name_ = "COSINUSDe"+in.name_;
    return out;
}

MogsInterval sin(const MogsInterval& in)
{
    MogsInterval * er_sin = in.get_son(SON_SIN_ERROR);
    MogsInterval out = approx_taylor_sin<MogsInterval,LazyVariable>(in,in.middle_) + *er_sin; // + *er;
    out.name_ = "SINUSDe"+in.name_;
    return out;
}

MogsInterval sign_of(const MogsInterval& in)
{
    MogsInterval*  tmp = interval_chief_.add_intermediate_pointer(in);
    tmp->name_ = "SignOf"+in.name_;
    tmp->prepare_son(SON_SIGNOF_ERROR);  
    MogsInterval * er = tmp->get_son(SON_SIGNOF_ERROR);    
    return *er;    
    
}

double pow2( double a)
{
    return pow(a,2);
}

LazyVariable pow2( const LazyVariable& a)
{
    return a*a;
}

MogsInterval pow2( const MogsInterval& a)
{
    return a*a;
}

Interval pow2( const Interval& a)
{
    if ( Inf(a)<0 && Sup(a)>0)
        return Hull( Hull( pow(Inf(a),2),pow(Sup(a),2)),0);
    return Hull( pow(Inf(a),2),pow(Sup(a),2));
}

bool compareMogsById(MogsInterval* a, MogsInterval* b)
{
    if (a->id_ < b->id_)    return true;
    if (a->id_ > b->id_)    return false;
    return a->name_ < b->name_;
}

