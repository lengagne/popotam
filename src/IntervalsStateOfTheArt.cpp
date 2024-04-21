#include "IntervalsStateOfTheArt.h"
#include <cassert>

IntervalsStateOfTheArt::IntervalsStateOfTheArt()
{
    //ctor
    name_ ="not_defined";
    value_ = 0.;
    operation_ = NONE;
}

IntervalsStateOfTheArt::IntervalsStateOfTheArt(const double & d)
{
    name_ ="double";
    value_ = d;
    operation_ = NONE;
}

IntervalsStateOfTheArt::IntervalsStateOfTheArt(const IntervalsStateOfTheArt & in)
{
    name_ = in.name_;
    value_ = in.value_;
    operation_ = in.operation_;
    dads_ = in.dads_;
    dad_coeff_ = in.dad_coeff_;
}

IntervalsStateOfTheArt::~IntervalsStateOfTheArt()
{
    //dtor
}

IntervalsStateOfTheArt::IntervalsStateOfTheArt(const Interval & in)
{
    value_ = in;
    name_ = "from_Interval";
    operation_ = NONE;
}

check_constraint IntervalsStateOfTheArt::contract(  const Interval& init,
                                                    const Interval& check)
{
    IntervalsStateOfTheArt dummy(check);
    check_constraint type =OVERLAP;
    if ( dummy.inside( *this))
        return INSIDE;


    if ( ! intersection( IntervalsStateOfTheArt(check)))
        return OUTSIDE;

    Interval tmp_inter;
    switch(operation_)
    {
        case(NONE):
                    return INSIDE;
        case(ADD):  assert(dads_.size()==2);
                    tmp_inter = value_- dads_[1]->value_;
                    if(dads_[0]->contract(tmp_inter) == OUTSIDE)
                        return OUTSIDE;
                    tmp_inter = value_- dads_[0]->value_;
                    if(dads_[1]->contract(tmp_inter) == OUTSIDE)
                        return OUTSIDE;
                    if( !intersection(dads_[0]->value_ + dads_[1]->value_ ))
                        return OUTSIDE;
                    break;
       case(SUB):   assert(dads_.size()==2);
                    tmp_inter = value_+ dads_[1]->value_;
                    if(dads_[0]->contract(tmp_inter) == OUTSIDE)
                        return OUTSIDE;
                    tmp_inter = dads_[0]->value_ - value_;
                    if(dads_[1]->contract(tmp_inter) == OUTSIDE)
                        return OUTSIDE;
                    if( !intersection(dads_[0]->value_ - dads_[1]->value_ ))
                        return OUTSIDE;
                    break;
       case(OPP):   assert(dads_.size()==1);
                    tmp_inter = - value_;
                    if(dads_[0]->contract(tmp_inter) == OUTSIDE)
                        return OUTSIDE;
                    if( !intersection(-dads_[0]->value_ ))
                        return OUTSIDE;

                    break;
        case(MUL):  assert(dads_.size()==2);
                    if ( !containts_zero(dads_[1]->value_))
                    {
                        tmp_inter = value_ / dads_[1]->value_;
                        if(dads_[0]->contract(tmp_inter) == OUTSIDE)
                            return OUTSIDE;
                    }
                    if ( !containts_zero(dads_[0]->value_))
                    {
                        tmp_inter = value_ / dads_[0]->value_;
                        if(dads_[1]->contract(tmp_inter) == OUTSIDE)
                            return OUTSIDE;
                    }
                    if( !intersection(dads_[0]->value_ * dads_[1]->value_ ))
                        return OUTSIDE;
                    break;
       case(MULD):  assert(dads_.size()==1);
                    if(dad_coeff_ != 0)
                    {
                        tmp_inter = value_/dad_coeff_;
                        if(dads_[0]->contract(tmp_inter) == OUTSIDE)
                            return OUTSIDE;
                    if( !intersection(dads_[0]->value_ * dad_coeff_))
                        return OUTSIDE;
                    }
                    break;
       case(COS):  assert(dads_.size()==1);
                    if(Inf(dads_[0]->value_) < 0 || Sup(dads_[0]->value_) > PI)
                    {
                        Interval tmp1 = ArcCos(value_);
                        tmp_inter = - tmp1;
                        IntervalsStateOfTheArt t1 = *dads_[0];
                        IntervalsStateOfTheArt t2 = *dads_[0];
                        bool b1,b2;
                        b1 =t1.intersection(tmp1);
                        b2 =t2.intersection(tmp_inter);
                        if(!b1&& !b2)
                            return OUTSIDE;
                        if(b1 && b2)
                            dads_[0]->value_ = Hull(t1.value_,t2.value_);
                        else if (b1)
                            dads_[0]->value_ = t1.value_;
                        else
                            dads_[0]->value_ = t2.value_;
                    }else
                    {
                        tmp_inter = ArcCos(value_);
                        if(!dads_[0]->intersection(tmp_inter))
                            return OUTSIDE;
                    }
                    value_ = Cos(dads_[0]->value_);
                    break;
       case(SIN):  assert(dads_.size()==1);
                  if(Inf(dads_[0]->value_) < PI/2 || Sup(dads_[0]->value_) > PI/2)
                    {
                        tmp_inter = ArcSin(value_);
                        Interval tmp1 = PI - tmp_inter;
                        Interval tmp2 = - PI - tmp_inter;
                        IntervalsStateOfTheArt t1 = *dads_[0];
                        IntervalsStateOfTheArt t2 = *dads_[0];
                        IntervalsStateOfTheArt t3 = *dads_[0];
                        bool b1,b2,b3;
                        b1 =t1.intersection(tmp1);
                        b2 =t2.intersection(tmp2);
                        b3 =t3.intersection(tmp_inter);
                        if(!b1&& !b2 && !b3)
                            return OUTSIDE;

                        if(b1)  dads_[0]->value_ = t1.value_;
                        else if (b2) dads_[0]->value_ = t2.value_;
                        else if (b3) dads_[0]->value_ = t3.value_;

                        if (b2) dads_[0]->value_ = Hull(dads_[0]->value_,t2.value_);
                        if (b3) dads_[0]->value_ = Hull(dads_[0]->value_,t3.value_);
                    }else
                    {
                        tmp_inter = ArcSin(value_);
                        if(!dads_[0]->intersection(tmp_inter))
                            return OUTSIDE;
                    }
                    value_ = Sin(dads_[0]->value_);
                    break;
    }
    return type;
}

bool IntervalsStateOfTheArt::inside(const IntervalsStateOfTheArt& in)
{
    return (Inf(in.value_) >= Inf(value_) && Sup(in.value_) <= Sup(value_));
}

bool IntervalsStateOfTheArt::intersection(const IntervalsStateOfTheArt& in)
{
    return Intersection(value_,value_,in.value_);
}

void IntervalsStateOfTheArt::update_from_inputs()
{
    switch(operation_)
    {
        case(NONE):
                    break;
        case(ADD):  //std::cout<<"ADD"<<std::endl;
                    dads_[0]->update_from_inputs();
                    dads_[1]->update_from_inputs();
                    value_ = dads_[0]->value_ + dads_[1]->value_;
                    break;
       case(SUB):   //std::cout<<"SUB"<<std::endl;
                    dads_[0]->update_from_inputs();
                    dads_[1]->update_from_inputs();
                    value_ = dads_[0]->value_ - dads_[1]->value_;
                    break;
       case(OPP):   //std::cout<<"OPP"<<std::endl;
                    dads_[0]->update_from_inputs();
                    value_ = -dads_[0]->value_;
                    break;
        case(MUL):  //std::cout<<"MUL"<<std::endl;
                    dads_[0]->update_from_inputs();
                    dads_[1]->update_from_inputs();
                    value_ = dads_[0]->value_ * dads_[1]->value_;
                    break;
       case(MULD):  //std::cout<<"MULD"<<std::endl;
                    dads_[0]->update_from_inputs();
                    value_ = dads_[0]->value_ * dad_coeff_;
                    break;
       case(COS):
                    dads_[0]->update_from_inputs();
                    value_ = Cos(dads_[0]->value_);
                    break;
       case(SIN):   dads_[0]->update_from_inputs();
                    value_ = Sin(dads_[0]->value_);
                    break;
        default:    std::cout<<"ERROR the case "<< operation_ <<" is not planned"<<std::endl;
    }
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator= (const IntervalsStateOfTheArt& in)
{
     if (this != &in)
     {
        name_ = in.name_;
        value_ = in.value_;
        dads_ = in.dads_;
        operation_ = in.operation_;
        dad_coeff_ = in.dad_coeff_;
     }
    return *this;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator- () const
{
    IntervalsStateOfTheArt out;
    out.value_ = -value_;
    out.dads_.push_back((IntervalsStateOfTheArt*) (this));
    out.operation_ = OPP;
    return out;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator- (const IntervalsStateOfTheArt& I) const
{
    IntervalsStateOfTheArt out;
    out. value_ = value_ - I.value_;
    out.dads_.push_back((IntervalsStateOfTheArt*)(this));
    out.dads_.push_back((IntervalsStateOfTheArt*) &I);
    out.operation_ = SUB;
    return out;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator+ (const IntervalsStateOfTheArt& I) const
{
    IntervalsStateOfTheArt out;
    out. value_ = value_ + I.value_;
    out.dads_.push_back((IntervalsStateOfTheArt*)(this));
    out.dads_.push_back((IntervalsStateOfTheArt*) &I);
    out.operation_ = ADD;
    return out;
}

// FIXME mettre au propre !!
void IntervalsStateOfTheArt::operator+= (const IntervalsStateOfTheArt& I)
{
    *this = *this+I;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator* (const double& d) const
{
    IntervalsStateOfTheArt out;
    out.value_ = value_ * d;
    out.dads_.push_back((IntervalsStateOfTheArt*)(this));
    out.operation_ = MULD;
    out.dad_coeff_ = d;
    return out;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator* (const IntervalsStateOfTheArt& in) const
{
    IntervalsStateOfTheArt out;
    out. value_ = value_ * in.value_;
    out.dads_.push_back((IntervalsStateOfTheArt*)(this));
    out.dads_.push_back((IntervalsStateOfTheArt*) &in);
    out.operation_ = MUL;
    return out;
}

IntervalsStateOfTheArt IntervalsStateOfTheArt::operator/ (const IntervalsStateOfTheArt& in) const
{
    IntervalsStateOfTheArt out;
    out. value_ = value_ / in.value_;
    out.dads_.push_back((IntervalsStateOfTheArt*)(this));
    out.dads_.push_back((IntervalsStateOfTheArt*) &in);
    out.operation_ = DIV;
    return out;
}


std::ostream& operator<< (std::ostream& stream, const IntervalsStateOfTheArt& inter)
{
    stream<<inter.name_ <<" = value_ "<<inter.value_<<" "<< inter.dads_.size()<<" dads";
    return stream;
}

IntervalsStateOfTheArt cos(IntervalsStateOfTheArt& in)
{
    IntervalsStateOfTheArt out;
    out.value_ = Cos(in.value_);
    out.name_ = "Cos(" + in.name_ +")";
    out.dads_.push_back(&in);
    out.operation_ = COS;
    return out;
}

IntervalsStateOfTheArt sin(IntervalsStateOfTheArt& in)
{
    IntervalsStateOfTheArt out;
    out.value_ = Sin(in.value_);
    out.name_ = "Sin(" + in.name_ +")";
    out.dads_.push_back(&in);
    out.operation_ = SIN;
    return out;
}
