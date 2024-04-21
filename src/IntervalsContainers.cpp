#include "IntervalsContainers.h"

std::vector<IntervalsStateOfTheArt*> ALL_IntervalS;

IntervalsContainers::IntervalsContainers()
{
    inter_ = new IntervalsStateOfTheArt();
    ALL_IntervalS.push_back(inter_);
}

IntervalsContainers::IntervalsContainers(const double & d)
{
    inter_ = new IntervalsStateOfTheArt(d);
    ALL_IntervalS.push_back(inter_);
}

IntervalsContainers::IntervalsContainers(const IntervalsContainers & in)
{
    inter_ = new IntervalsStateOfTheArt(*in.inter_);
    ALL_IntervalS.push_back(inter_);
}

IntervalsContainers::~IntervalsContainers()
{
    // we do not erase the point !!!
}

IntervalsContainers::IntervalsContainers(const Interval & in)
{
    inter_ = new IntervalsStateOfTheArt(in);
    ALL_IntervalS.push_back(inter_);
}

IntervalsContainers IntervalsContainers::operator= (const IntervalsContainers& in)
{
    inter_ = new IntervalsStateOfTheArt(*in.inter_);
    ALL_IntervalS.push_back(inter_);
    return *this;
}

IntervalsContainers IntervalsContainers::operator- () const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = - *inter_;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

IntervalsContainers IntervalsContainers::operator- (const IntervalsContainers& I) const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = *inter_ - *I.inter_;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

IntervalsContainers IntervalsContainers::operator+ (const IntervalsContainers& I) const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = *inter_ + *I.inter_;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

// FIXME mettre au propre !!
void IntervalsContainers::operator+= (const IntervalsContainers& I)
{
    *this = *this+I;
}

void IntervalsContainers::operator-= (const IntervalsContainers& I)
{
    *this = *this-I;
}

IntervalsContainers IntervalsContainers::operator* (const double& d) const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = *inter_ * d;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

IntervalsContainers IntervalsContainers::operator* (const IntervalsContainers& in) const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = *inter_ * *in.inter_;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

IntervalsContainers IntervalsContainers::operator/ (const IntervalsContainers& in) const
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = *inter_ / *in.inter_;
    ALL_IntervalS.push_back(out.inter_);
    return out;
}


std::ostream& operator<< (std::ostream& stream, const IntervalsContainers& I)
{
    stream<<*I.inter_;
    return stream;
}

IntervalsContainers cos(const IntervalsContainers& in)
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = cos(*in.inter_);
    ALL_IntervalS.push_back(out.inter_);
    return out;
}

IntervalsContainers sin(const IntervalsContainers& in)
{
    IntervalsContainers out;
    out.inter_ = new IntervalsStateOfTheArt();
    *out.inter_ = sin(*in.inter_);
    ALL_IntervalS.push_back(out.inter_);
    return out;
}
