#ifndef AbstractSolver_H
#define AbstractSolver_H

#define DEFAULT_SAVE_EACH_ITER 10000000

#include <iostream>
#include <fstream>

#include "AbstractCSP.h"
#include "Result.h"
#include "SolverOptions.h"

typedef struct param_optim
{
    uint nb_bissections;
    uint nb_valid_boxes;
    uint nb_possible_boxes;
    uint nb_intermediate;
//     double ignored_space_;
    double computation_time;
    double computation_time_wo_prep;
    double optim;
    bool solution_found = true;
} param_optim;



class AbstractSolver
{
    public:
//         bool bissect(   const Result& in,
//                         Result &out1, 
//                         Result& out2);
        bool bissect(   const Result& in,
                        std::vector<Result>& Pile)     ;
        

        bool check_size(   const Result& in);
        
        /// test if the Interval in is INSIDE/OUTSIDE or OVERLAPPING the bound
        check_constraint test_Interval( const Interval &in ,
                                       const Interval & bound);
        

        void close_files();
        
        virtual void evaluate(  const std::vector<Interval>& in,
                                std::vector<Interval> & out) = 0;
                                
        virtual void init_option();
                                
        bool load_save_filename( const std::string& s,
                                const Result & res  // is initialized
        );     
        
        virtual param_optim solve();
        
        void prepare_files(const std::string& filename);
                                
        void save_current_state( const std::string& filename = "current_state_optim");

        void set_precision_gnuplot(const double a)
        {
            precision_gnuplot_ = a;
        }

        void set_print(bool in)
        {
            print_ = in;
        }
        
        virtual param_optim set_results();
        
        virtual void set_options( SolverOptions options)
        {
            options_ = options;
        }
        
        void set_save_filename( const std::string& s);
        
        std::string update_filename( const std::string& filename); // check the filename and update version.

    protected:
        
        virtual param_optim solve_optim(double eps=1e-3) = 0;
        
        SolverOptions options_;
        
        AbstractCSP *pb_;

        std::vector<Result> current_vector_;
        
        std::vector<Result> results;
        std::vector<Interval> bounds_,bounds_input_;

        unsigned int nb_valid_box_;
        unsigned int nb_maybe_box_;
//         double ignored_space_;
        
        unsigned int nb_intermediate_ = 0;

        unsigned int nb_fun_, nb_var_;

        double precision_;
        unsigned long long int cpt_iter_;
        unsigned long int save_each_iter_ = DEFAULT_SAVE_EACH_ITER;
        unsigned long int saved_iter_ = 0;
        
        double current_time_;
        double start_preparation_time_;
        double start_computation_time_;
        double preparation_duration_;
        
        double previous_time_ = 0;
        
        double optim_crit_;
        bool find_one_feasible_ =false;
        Result optim_;

        bool print_ = false;
        
        uint bissection_type_ = 0;
        
        bool save_and_load_ = false;
        std::string save_filename_ = "no_save";
               
    private:

        bool file_open = false;
        std::ofstream file_safe_;
        std::ofstream file_maybe_;
        std::ofstream file_gnuplot_input_;
        std::ofstream file_gnuplot_output_;
        double precision_gnuplot_ = 0.9e-3;
        std::string file_prefix_;
        unsigned int cpt_rec_input_, cpt_rec_output_;
        
};

#endif // AbstractSolver_H
