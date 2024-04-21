#ifndef ABSTRACTCSP_H
#define ABSTRACTCSP_H

#include <vector>
#include "MogsInterval.h"
#include "IntervalsContainers.h"
#include "utils.h"

inline Interval sqrt(const Interval&in)
{
    return Sqrt(in);
}


class AbstractCSP
{
    public:
        AbstractCSP(){}
        virtual ~AbstractCSP() {}

        virtual void function( std::vector<double>& input,
                               std::vector<double>& output) = 0;

        virtual void function( std::vector<Interval>& input,
                               std::vector<Interval>& output) = 0;

        virtual void function( std::vector<MogsInterval>& input,
                               std::vector<MogsInterval>& output) = 0;

        virtual void function( std::vector<IntervalsContainers>& input,
                               std::vector<IntervalsContainers>& output) = 0;

        std::string get_problem_name() const
        {
            return pb_name_;
        }

        std::vector<Interval> get_bound()
        {
            return bound_;
        }

        std::vector<Interval> get_input()
        {
            return input_;
        }

        unsigned int get_nb_in() const
        {
            return nb_var_;
        }

        unsigned int get_nb_out() const
        {
            return nb_fun_;
        }

        bool get_criteria()const
        {
            return get_criteria_;
        }

        virtual void init() =0;

        virtual void prepare_non_linear( std::vector<MogsInterval>& in) = 0;

    protected:
        bool get_criteria_ = false;

        std::string pb_name_;

        unsigned int nb_var_;   // number of input
        unsigned int nb_fun_;   // number of output

        std::vector<Interval> input_, bound_;

        std::vector<std::string> input_name_, output_name_;
};

#endif // ABSTRACTCSP_H
