#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>
#include <limits>
#include <Eigen/Core>
#include "utils.h"

#define PI 3.14159265359

class Interval
{
    public:
        Interval();
        Interval(const double & in);
    private:
        // those functions are private because the input must be set in the proper order
        // otherwie use Hull
        Interval(const double & a,const double & b);
        Interval(const Interval & a,const Interval & b);

    public:
        virtual ~Interval();

        bool contains(double value) const;

        void update(const Interval& in);

        friend double Inf(const Interval & in);
        friend double Sup(const Interval & in);

        friend Interval Cos(const Interval & in);
        friend Interval Sin(const Interval & in);
        friend Interval Sqrt(const Interval & in);
        friend Interval ArcCos(const Interval & in);
        friend Interval ArcSin(const Interval & in);
        friend Interval SignOf( const Interval& a);

        friend Interval Power(const Interval & in, int order);

        void operator *= (const Interval & in);
        void operator += (const Interval & in);
        void operator -= (const Interval & in);

        bool operator == (const Interval & in) const;
        bool operator != (const Interval & in) const;

        Interval operator + (const double & in) const;
        Interval operator * (const double & in) const;
        Interval operator + (const Interval & in) const;
        Interval operator - (const Interval & in) const;
        Interval operator * (const Interval & in) const;
        Interval operator / (const Interval & in) const;

        void operator /= (const double& in)
        {
            *this = *this * (1./in);
        }

        friend Interval operator - (const Interval & in);

        friend std::ostream& operator<<(std::ostream& os, const Interval& obj);

        friend double Diam(const Interval & a);
        friend double Mid(const Interval & a);
        friend Interval abs(const Interval &a);
        
        friend Interval Hull(const double & in);
        friend Interval Hull(const double & a,const double & b);
        friend Interval Hull(const Interval & a,const Interval & b);

        friend bool Intersection(   const Interval& a, const Interval& b);
        friend bool Intersection(  Interval & res1, const Interval& a, const Interval& b);

    protected:
    private:
    double inf_, sup_;
};

inline Interval Hull(const double & in)
{
    return Interval(in);
}

inline Interval Hull(const double & a,const double & b)
{
    return Interval(MIN(a,b),MAX(a,b));
}

inline Interval Hull(const Interval & a,const Interval & b)
{
    return Interval(a,b);
}

inline Interval operator + (const double & a, const Interval & b)
{
    return b+a;
}

inline Interval operator * (const double & a, const Interval & b)
{
    return b*a;
}

inline Interval operator - (const double & a, const Interval & b)
{
    return -b+a;
}

inline Interval pow( const Interval & a, uint b)
{
    Interval out = 1;
    for (uint i=0;i<b;i++)
        out = out * a;
    return out;
}

inline  Interval abs(const Interval &a)
{
    if (a.inf_ < 0 & a.sup_>= 0)
        return Hull( MAX(a.inf_,a.sup_),0.);
    if (a.inf_>=0)
        return a;
    if (a.sup_<0)
        return a;
}

inline Interval cos (const Interval& in)
{
    return Cos(in);
}

inline Interval sin (const Interval& in)
{
    return Sin(in);
}

inline Interval sign_of (const Interval& in)
{
    return SignOf(in);
}

// #define MAX(a,b) (((a)>(b))?(a):(b))

#endif // INTERVAL_H
