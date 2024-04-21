#include "Interval.h"

Interval::Interval():inf_(-std::numeric_limits<double>::max()),sup_(std::numeric_limits<double>::max())
{
    //ctor
}

Interval::Interval(const double & in):inf_(in),sup_(in)
{
}

Interval::Interval(const double & a,const double & b)
{
    inf_ = a;
    sup_ = b;
}

Interval::Interval(const Interval & a,const Interval & b)
{
    inf_ = MIN(a.inf_,b.inf_);
    sup_ = MAX(a.sup_,b.sup_);
}

Interval::~Interval()
{
    //dtor
}

bool Interval::contains(double value) const
{
    return (inf_<= value && sup_>= value);
}

void Interval::update(const Interval& in)
{
    inf_ = in.inf_;
    sup_ = in.sup_;
}

Interval Interval::operator + (const double & in) const
{
    return Interval(inf_+in, sup_+in);
}

Interval Interval::operator * (const double & in) const
{
    if (in>0)
        return Interval(inf_*in, sup_*in);
    return Interval(sup_*in,inf_*in);
}


Interval Interval::operator + (const Interval & in) const
{
    return Interval(inf_+in.inf_,sup_+in.sup_);
}

Interval Interval::operator - (const Interval & in) const
{
    return Interval(inf_-in.sup_,sup_-in.inf_);
}

Interval Interval::operator * (const Interval & in) const
{
    Eigen::Matrix<double,4,1> val;
    val(0) = inf_ * in.inf_;
    val(1) = inf_ * in.sup_;
    val(2) = sup_ * in.inf_;
    val(3) = sup_ * in.sup_;
    return Interval(val.minCoeff(),val.maxCoeff());
}

Interval Interval::operator / (const Interval & in) const
{
//     std::cout<<"Interval / "<< *this<<" / "<< in <<std::endl;
    if (in.contains(0.))
        return Interval();
//     std::cout<<" = "<< *this * Hull(1./in.inf_, 1./in.sup_) <<std::endl;
    return *this * Hull(1./in.inf_, 1./in.sup_);
}

bool Interval::operator == (const Interval & in) const
{
    return ( inf_ == in.inf_ && sup_ == in.sup_);
}

bool Interval::operator != (const Interval & in) const
{
    return !( inf_ == in.inf_ && sup_ == in.sup_);
}

void Interval::operator += (const Interval & in)
{
    inf_ += in.inf_;
    sup_ += in.sup_;
}

void Interval::operator -= (const Interval & in)
{
    inf_ -= in.inf_;
    sup_ -= in.sup_;
}


void Interval::operator *= (const Interval & in)
{
    Eigen::Matrix<double,4,1> val;
    val(0) = inf_ * in.inf_;
    val(1) = inf_ * in.sup_;
    val(2) = sup_ * in.inf_;
    val(3) = sup_ * in.sup_;
    inf_ = val.minCoeff();
    sup_ = val.maxCoeff();
}

///////////////////////////////

Interval operator - (const Interval & in)
{
    return Interval(-in.sup_,-in.inf_);
}

Interval ArcCos(const Interval & in)
{
    return Interval( std::acos(in.sup_),std::acos(in.inf_));
}

Interval Cos(const Interval & in)
{
    if(Diam(in)>2*PI)
        return Interval(-1,1);
    Interval input = in;
    while(input.inf_<0) input+= 2*PI;
    while(input.inf_>2*PI) input-= 2*PI;
    if (input.contains(PI) && input.contains(2*PI))
        return Interval(-1.,1.0);
    else if (input.contains(PI))
    {
        return Interval(-1, MAX(cos(input.inf_),cos(input.sup_)));
    }
    else if (input.contains(2*PI))
    {
        return Interval(MIN(cos(input.inf_),cos(input.sup_)),1.0);
    }

    return Hull( cos(input.inf_),cos(input.sup_));
}

double Inf(const Interval & in)
{
    return in.inf_;
}

bool Intersection(  const Interval& a,
                    const Interval& b)
{
    if(a.sup_ < b.inf_ || a.inf_ > b.sup_)
    {
        return false;
    }
    return true;
}


bool Intersection(Interval & res1,
                            const Interval& a,
                            const Interval& b)
{
    if(a.sup_ < b.inf_ || a.inf_ > b.sup_)
    {
        return false;
    }
    res1 = Interval (MAX(a.inf_,b.inf_),MIN(a.sup_,b.sup_));
    return true;
}

double Diam(const Interval & a)
{
    return a.sup_ - a.inf_;
}

double Mid(const Interval & a)
{
    return (a.sup_ + a.inf_)/2.;
}

Interval Power(const Interval & in, int order)
{
    double mini = pow(in.inf_,order);
    double maxi = pow(in.sup_,order);
    if(in.inf_<0 && in.sup_>0)
    {
        return Interval(0,MAX(mini,maxi));
    }else
    {
        return Interval(mini,maxi);
    }
}

double Sup(const Interval & in)
{
    return in.sup_;
}

Interval ArcSin(const Interval & in)
{
    return Interval( std::asin(in.inf_),std::asin(in.sup_));
}

Interval Sin(const Interval & in)
{
    return Cos( in-PI/2.);
}

Interval SignOf( const Interval& a)
{
    if (Sup(a) < 0)
        return -1;
    if (Inf(a) > 0)
        return 1;
    return Interval(-1,1);
}

Interval Sqrt(const Interval & in)
{
    if (in.inf_<0)
    {
        std::cerr<<__FILE__<<": "<< __LINE__<<std::endl;
        std::cerr<<"Error in function Sqrt ask for negative value"<<std::endl;
        exit(0);
    }
    return Interval (sqrt(in.inf_), sqrt(in.sup_));
}

std::ostream& operator<<(std::ostream& os, const Interval& obj)
{
     std::cout.precision(10);
    os<<"["<<obj.inf_<<":"<<obj.sup_<<"]";
    return os;
}


