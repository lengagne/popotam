#ifndef EvalCurrent_H
#define EvalCurrent_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"
#include "IntervalEstimator.h"


class EvalCurrent
{
    public:
        
        EvalCurrent( AbstractCSP* pb,
                     AbstractBasisFunction* bf
        );
        
        void evaluate(Result& value);
        
        uint get_nb_intermediate()
        {
            return nb_intermediate_;
        }
        
        void init();
        
        bool process_current(Result& value, optim_info& info);
        
        bool process_current_with_score( Result& value, double & score);
       
        

    private:
        
        
        void compute_intermediate_for(uint num_function);
        
        void get_all_intermediate_dependancies( const std::list<uint>& id_to_add,
                                                 std::list<uint> & full_list);        
        
        void set_current( const Result& value);
                
        Result current_value_;
        
        bool solve_optim_ = true;
        
        uint nb_var_, nb_fun_;
        unsigned int nb_intermediate_ = 0;

        
        std::vector<Interval> bounds_,bounds_input_;
        
        std::vector<MogsInterval> input_Interval,output_Interval;
        std::vector<MogsInterval> Intermediate_to_compute;
        std::vector<MogsInterval> Intermediate_to_update;
        std::vector<IntervalEstimator*> infos;
        std::vector<IntervalEstimator*> infos_intermediate_update;
        
        std::vector<bool> intermediate_updated_;
        std::vector< std::list<uint> > intermediate_needed_;
        
        IntervalEstimator* info_crit_;

        std::vector<MogsInterval> intermediate_;

};

#endif // BasisFunctionSolver_H
