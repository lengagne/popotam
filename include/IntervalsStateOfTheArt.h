#ifndef IntervalsStateOfTheArt_H
#define IntervalsStateOfTheArt_H

#include <vector>
#include "MogsInterval.h"

#define PI 3.14159265359

typedef enum  { ADD, SUB, OPP, MUL, MULD, DIV, COS, SIN ,NONE}  operation_type;

class IntervalsStateOfTheArt
{
    public:
        IntervalsStateOfTheArt();

        IntervalsStateOfTheArt(const Interval & in);

        IntervalsStateOfTheArt(const IntervalsStateOfTheArt & in);

        IntervalsStateOfTheArt(const double & d);

        check_constraint contract(const Interval & init, const Interval& check);

        check_constraint contract(const Interval & in)
        {
            return contract(in,in);
        }

        bool containts_zero(const Interval& I)
        {
            return (Inf(I) <= 0 && Sup(I) >= 0);
        }

        void init(const Interval & inter,
                  const std::string& name ="none")
        {
            value_ = inter;
            name_ = name;
        }

        /// check if the Interval in is inside the current Interval.
        bool inside(const IntervalsStateOfTheArt& in);

        bool intersection(const IntervalsStateOfTheArt& in);

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        void update(const Interval& inter)
        {
             value_ = inter;
        }

        void update_from_inputs();

        virtual ~IntervalsStateOfTheArt();

        IntervalsStateOfTheArt operator= (const IntervalsStateOfTheArt& I);

        IntervalsStateOfTheArt operator+ (const IntervalsStateOfTheArt& I) const;

        IntervalsStateOfTheArt operator- (const IntervalsStateOfTheArt& I) const;

        IntervalsStateOfTheArt operator* (const double& d) const;

        IntervalsStateOfTheArt operator- () const;

        void operator+= (const IntervalsStateOfTheArt& I);

        inline IntervalsStateOfTheArt operator/ (const double& d) const
        {
            return *this * (1./d);
        }

        IntervalsStateOfTheArt operator* (const IntervalsStateOfTheArt& in) const;
        
        IntervalsStateOfTheArt operator/ (const IntervalsStateOfTheArt& in) const;

        friend std::ostream& operator<< (std::ostream& stream, const IntervalsStateOfTheArt& inter);
        std::string name_;

        Interval value_;

        std::vector<IntervalsStateOfTheArt*> dads_; // Interval to contain error from sin and cos
        double dad_coeff_;

        operation_type operation_;
};

inline IntervalsStateOfTheArt operator*( const double & d, const IntervalsStateOfTheArt& in)
{
    return in*d;
}

inline IntervalsStateOfTheArt operator+( const double & d, const IntervalsStateOfTheArt& in)
{
    return in+d;
}


IntervalsStateOfTheArt cos(IntervalsStateOfTheArt& in);

IntervalsStateOfTheArt sin(IntervalsStateOfTheArt& in);


#endif // IntervalsStateOfTheArt_H

