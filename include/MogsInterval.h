#ifndef MogsInterval_H
#define MogsInterval_H

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "LazyCPP.hpp"
#include <utils.h>
#include "Interval.h"
#include "Kronecker.h"


#define MAXSIZE 50

#define LazyVariable LazyVariable

// typedef double Real;

typedef enum {SON_COS_ERROR, SON_SIN_ERROR, SON_SIN_COS_ERROR,SON_SIGNOF_ERROR /*,DIVIDE_ERROR,*/} sons_type;

typedef enum  { INSIDE, OUTSIDE, OVERLAP }  check_constraint;

inline bool zero_inside_inter( const Interval& in)
{
    return (Inf(in)<= 0 && Sup(in) >= 0);
}
class MogsInterval;


struct sons;

class mem{
public:
    mem(){    sons.clear(); };

    mem(const mem& m);

    mem(MogsInterval* i);

    mem(MogsInterval* i, unsigned int degree);

    bool can_contract() const;

    check_constraint contract(const Interval& in);

    void compute_recurence();
    
    /// return the number of occurence
    uint get_nb(MogsInterval* ref);

    bool is_contractable(unsigned int contraction_mode=0) const;

    bool is_it_output() const;

    bool contains_error() const;
    
    void remove_one_ref( MogsInterval*ref);
    
    bool is_double()
    {
        return sons.size();
    }

//    LazyVariable value;
    std::list<MogsInterval*> sons;
    std::map<MogsInterval*, unsigned int> recurence;    //need to call compute_recurence
    bool recurence_done = false;
};

std::ostream& operator<< (std::ostream& stream, const mem& m);

bool operator==(const mem& a, const mem&b);


/** Returns true if the two mems are on the same variables **/
bool compare(const mem& m1,const mem& m2);

void MogsIntervalInit();

class MogsInterval
{
    public:
        MogsInterval();

        MogsInterval(const MogsInterval & in);

        MogsInterval(const LazyVariable & in);

        MogsInterval(const double & d);

        MogsInterval(const Interval& val,
                     const std::string& name="undefined");

        virtual ~MogsInterval();

        static MogsInterval add_intermediate(const MogsInterval & inter,
                                             const unsigned int M=MAXSIZE);
        
        void add (const LazyVariable&in, mem * m);

        void compute_from_polynomial();

        check_constraint contract_of_order(const Interval& in,
                                           unsigned int degree);
        
        /// return the derivative regarding the reference, 
        /// we assume the reference is properly set
        MogsInterval derivative(MogsInterval* reference) const;

        // return *this - in without use of intermediate variables
        MogsInterval dual ( const MogsInterval& in) const;

        void get_dependancies_with_order(std::vector<MogsInterval*> &dep,
                                         std::vector<unsigned int> & order ) const;     
                                         
        /// return the reference variable 
        /// failed in there are several
        void get_reference( MogsInterval** ref) const;                                  

        unsigned int get_maximal_order(const MogsInterval& var) const;

        /// return the list of the intermediate variables
        void get_intermediate(  std::vector<MogsInterval*> & inters,
                                std::vector<unsigned int > & ids,
                                bool with_error = false) const;

        /// return the son (if does not exists create one)
        MogsInterval* get_son(const sons_type& type) const;


        void prepare_son(const sons_type& type);

        mem* chief_check_input(mem* in) const;
        
        /// return the pointeur to the save mem
        mem* get_mem_pointeur(const mem& m) const;

        unsigned int get_id_intermediate() const
        {
            return id_intermediate_;
        }
        
        static unsigned int get_nb_intermediate();
        
        

        static void get_intermediate_to_compute(std::vector<MogsInterval> & out);

        static void get_intermediate_to_update(std::vector<MogsInterval> & out);

//         static unsigned int get_max_level();
        
        LazyVariable get_mid_value() const
        {
            return get_value(get_double_mem());
        }

        LazyVariable get_value(mem* m) const;

        mem* get_double_mem()const;

        // get the interval value from the ref and reset the ref to Hull(-1,1)
        bool get_contract_from_ref( std::vector<Real>& v,
                                    Interval &i);

        unsigned int get_nb_value()const
        {
            return dependances_.size();
        }

        /// guess the number of component needs in the Kronecker products
        unsigned int guess_size() const;

        void init(const Interval& val,
                     const std::string& name="undefined");

        /// check if the current Interval is inside the in Interval.
        bool inside(const Interval& in);

        void is_error()
        {
            is_an_error_ = true;
        }

        bool is_it_error() const
        {
            return is_an_error_;
        }

        bool is_it_intermediate() const
        {
            return is_an_intermediare;
        }
        
        bool is_it_input() const
        {
            return is_input_;
        }        

        bool is_it_output() const
        {
            return is_output_;
        }

        bool intersection(const Interval& in, bool contract_son=true);

        bool rely_on_error()const;

        void set_name(const std::string& name ="undefined")
        {
            name_ = name;
//             middle_.set_name("MiddleOf"+name_);
//             diam_.set_name("DiamOf"+name_);
            middle_ = LazyVariable("MiddleOf"+name_);
            diam_ = LazyVariable( "DiamOf"+name_);
        }

        void set_as_output()
        {
            is_output_ = true;
            if(ref_)    ref_->is_output_ = true;
        }

//         void update(const Interval & in, std::vector<Real>& v);
        void update(const Interval & in);

//         void update_error_cos(const Interval & in, std::vector<Real>& v);
//         void update_error_sin(const Interval & in, std::vector<Real>& v);
//         void update_error_sin_cos(const Interval & in, std::vector<Real>& v);
//         void update_error_sign_of(const Interval & in, std::vector<Real>& v);
//         void update_error_division(const Interval & in, std::vector<Real>& v);

//         void update_vector_from_ref(std::vector<Real>& v);
        
        void update_error_cos(const Interval & in);
        void update_error_sin(const Interval & in);
        void update_error_sin_cos(const Interval & in);
        void update_error_sign_of(const Interval & in);
        void update_error_division(const Interval & in);        

        bool operator== (const MogsInterval& I);
        
        bool operator== (const double in);

        MogsInterval operator= (const MogsInterval& I);
        MogsInterval operator+ (const MogsInterval& I) const;
        MogsInterval operator- (const MogsInterval& I) const;
        MogsInterval operator* (const MogsInterval& in) const;
//         MogsInterval operator/ (const MogsInterval& in) const;
        MogsInterval operator* (const double& d) const;

        MogsInterval operator* (const LazyVariable& d) const;

        inline void operator*= (const MogsInterval& in) 
        {
            *this = *this *in;
        }

        MogsInterval operator- () const;
        void operator+= (const MogsInterval& I);
        void operator-= (const MogsInterval& I);

        void operator /= (const double& d)
        {
            *this = *this / d;
        }

        inline MogsInterval operator/ (const double& d) const
        {
            return *this * (1./d);
        }
        
        bool operator== (const double &a) const;
        
        bool operator== (const MogsInterval &a) const;
//         {
//             return value_ == a.value_;
//         }

        friend MogsInterval cos(const MogsInterval& in);
        friend MogsInterval sin(const MogsInterval& in);
        friend MogsInterval sign_of(const MogsInterval& in);
        friend MogsInterval inverse(const MogsInterval& in);

        friend std::ostream& operator<< (std::ostream& stream, const MogsInterval& inter);
        friend std::ostream& operator<< (std::ostream& stream, const mem& m);

        LazyVariable middle_;
        LazyVariable diam_;

        MogsInterval * ref_=NULL;

        // data
        std::string name_;
        Interval value_;
        std::map<mem*,LazyVariable> dependances_;

        std::vector<sons> the_sons_;
        bool is_output_ = false;
        bool is_input_ = false;

//         unsigned int level_ = 0;    // level in the temporary variables

//    private:
        bool is_an_error_ = false;
        bool is_an_intermediare = false;
        unsigned long long id_ = 0;
        unsigned int id_intermediate_ = 0;

};

// return the maximal dependancies for each outut (and return into 1)
void merge_dependancies(std::vector<MogsInterval*> &dep1,
                        std::vector<MogsInterval*> &dep2,
                        std::vector<unsigned int> & order1,
                        std::vector<unsigned int> & order2);


template <typename T1, typename T2>
T1 approx_taylor_division( const T1& num, const T2& middle_num, const T1& den, const T2& middle_den)
{    
    std::vector<MogsInterval*> dep_num, dep_den;
    std::vector<unsigned int> order_num,order_den;
    
    num.get_dependancies_with_order(dep_num,order_num);
    den.get_dependancies_with_order(dep_den,order_den);
    
    merge_dependancies( dep_num,dep_den, order_num,order_den);
   
    
    T1 out = middle_num/middle_den;
    return  out;
}



template <typename T1, typename T2>
T1 approx_taylor_cos( const T1& value, const T2& middle)
{
    T1 a = value - middle;
    return  cos( middle)
            - a*  sin(middle);
//             - a*a*  cos(middle) /2;
//            + a*a*a*  sin(middle) /6;
//            + a*a*a*a*  cos(middle) /24
//            - a*a*a*a*a*  sin(middle) /120;
}

template <typename T1, typename T2>
T1 approx_taylor_sin( const T1& value, const T2& middle)
{
    T1 a = value - middle;
    return  sin( middle)
            + a*  cos(middle);
//             - a*a* sin(middle) /2;
//             - a*a*a* cos(middle) /6;
//            + a*a*a*a* sin(middle) /24
//            + a*a*a*a*a*  cos(middle) /120;
}


typedef struct sons
{
    sons_type type;
    MogsInterval* son;
}sons;


inline MogsInterval operator*( const double & d, const MogsInterval& in)
{
    return in*d;
}

inline MogsInterval operator+( const double & d, const MogsInterval& in)
{
    return in+d;
}

inline MogsInterval operator-( const LazyVariable & t, const MogsInterval& in)
{
    return MogsInterval(t)-in;
}

inline MogsInterval operator+( const LazyVariable & t, const MogsInterval& in)
{
    return MogsInterval(t)+in;
}



#endif // MogsInterval_H
