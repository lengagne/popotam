#ifndef IntervalsContainers_H
#define IntervalsContainers_H

#include "IntervalsStateOfTheArt.h"

extern std::vector<IntervalsStateOfTheArt*> ALL_IntervalS;

class IntervalsContainers
{
    public:
        IntervalsContainers();

        IntervalsContainers(const Interval & in);

        IntervalsContainers(const IntervalsContainers & in);

        IntervalsContainers(const double & d);

        IntervalsContainers(const std::string& name,
                  double inf = -1e9,
                  double sup = 1e9);

        static void clear_all()
        {
            for (int i=0;i<ALL_IntervalS.size();i++)
                delete ALL_IntervalS[i];
            ALL_IntervalS.clear();
        }

        /** Return true if the contraction is possible
        */
        check_constraint contract(const Interval & init, const Interval& check)
        {
            return inter_->contract(init,check);
        }

        void init(const Interval & inter,
                  const std::string& name ="none")
        {
            inter_->init(inter,name);
        }

        void set_name(const std::string& name)
        {
            inter_->set_name(name);
        }

        void update(const Interval& inter)
        {
            inter_->update(inter);
        }

        void update_from_inputs()
        {
            inter_->update_from_inputs();
        }

        Interval value() const
        {
            return inter_->value_;
        }

        virtual ~IntervalsContainers();

        IntervalsContainers operator= (const IntervalsContainers& I);

        IntervalsContainers operator+ (const IntervalsContainers& I) const;

        IntervalsContainers operator- (const IntervalsContainers& I) const;

        IntervalsContainers operator* (const double& d) const;

        IntervalsContainers operator- () const;

        void operator+= (const IntervalsContainers& I);
        void operator-= (const IntervalsContainers& I);

        inline void operator/= (const double& d)
        {
            *this = *this / d;
        }

        inline void operator*= (const IntervalsContainers& I)
        {
            *this = *this * I;
        }
        
        
        inline IntervalsContainers operator/ (const double& d) const
        {
            return *this * (1./d);
        }

        IntervalsContainers operator* (const IntervalsContainers& in) const;
        
        IntervalsContainers operator/ (const IntervalsContainers& in) const;

        friend std::ostream& operator<< (std::ostream& stream, const IntervalsContainers& inter);

        IntervalsStateOfTheArt* inter_;

};

inline IntervalsContainers operator*( const double & d, const IntervalsContainers& in)
{
    return in*d;
}

inline IntervalsContainers operator+( const double & d, const IntervalsContainers& in)
{
    return in+d;
}


IntervalsContainers cos(const IntervalsContainers& in);

IntervalsContainers sin(const IntervalsContainers& in);


#endif // IntervalsContainers_H

