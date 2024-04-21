#ifndef MOGSINTERVALCHIEF_H
#define MOGSINTERVALCHIEF_H

#include "MogsInterval.h"


class MogsIntervalChief
{
    public:
        MogsIntervalChief();
        virtual ~MogsIntervalChief();

        MogsInterval * add_intermediate_pointer( const MogsInterval& input);
        
        MogsInterval add_intermediate(const MogsInterval& inter, unsigned int M=MAXSIZE);

        mem* check_input(mem* in);

        mem* get_mem(const mem& in);

        mem* get_double()
        {
//            std::cout<<"double_value_ = "<< double_value_ <<std::endl;
            return double_value_;
        }

        unsigned int get_nb_intermediate()
        {
            return intermediate_.size();
        }

        void get_intermediate_to_compute(std::vector<MogsInterval>& out)
        {
            out = intermediate_;
        }

        void get_intermediate_to_update(std::vector<MogsInterval>& out)
        {
            out = intermediate_in_;
        }

        unsigned int get_max_level();
        
        void reset();

    protected:

    private:

        std::list<mem> monomials_;
        mem* double_value_;

        std::vector<MogsInterval> intermediate_;
        std::vector<MogsInterval> intermediate_in_;

        std::map< unsigned long long , MogsInterval> memory_;

};

#endif // MOGSINTERVALCHIEF_H
