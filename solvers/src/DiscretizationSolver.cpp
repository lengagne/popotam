#include "DiscretizationSolver.h"
#include <string.h>

double random(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

DiscretizationSolver::DiscretizationSolver(AbstractCSP* pb)
{
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    std::cout<<"Creating DiscretizationSolver for problem : "<< pb_->get_problem_name()<<std::endl;
}

DiscretizationSolver::~DiscretizationSolver()
{
    close_files();
    results.clear();
}

void DiscretizationSolver::evaluate(const std::vector<Interval> &in,
                                    std::vector<Interval> &out)
{
    std::cerr<<"Function evaluate not defined yet for DiscretizationSolver"<<std::endl;

    std::vector<double> input(nb_var_);
    for (int i=0;i<nb_var_;i++)
        input[i] = Inf(in[i]);

    std::vector<double> output(nb_fun_);
    pb_->function(input,output);
    for (int i=0;i<nb_fun_;i++)
        out[i] = output[i];

    std::vector<unsigned int> cpt(nb_var_);
    for (int i=0;i<nb_var_;i++) cpt[i] = 0;

    unsigned int a = 0;
    bool test = true;
    for (int k=0;k<nb_discret_;k++)
    {
        for (int i=0;i<nb_var_;i++)
            input[i] = random(Inf(in[i]), Sup(in[i]));

        pb_->function(input,output);
        for (int i=0;i<nb_fun_;i++)
            out[i] = Hull(out[i], output[i]);
    }
}



param_optim DiscretizationSolver::solve_optim(double eps)
{
    double optim_crit = std::numeric_limits<double>::max();
    bool find_one_feasible =false;
    std::cout<<"DiscretizationSolver::solve_csp"<<std::endl;
    double ts = get_cpu_time();
    double tsglobal = ts;
    precision_ = eps;

    std::vector<Result> current;
    bounds_ = pb_->get_bound();
    bounds_input_ = pb_->get_input();

    std::vector<double> dinput_(nb_var_), doutput_(nb_fun_+ pb_->get_criteria());

    
    Result to_store(pb_->get_input(), nb_fun_, pb_->get_criteria());

    std::vector<unsigned int> nb_steps_(nb_var_), mem_index_(nb_var_);
    for (int i=0;i<nb_var_;i++)
    {
        nb_steps_[i] = Diam(bounds_input_[i]/eps) +1 ;
        mem_index_[i] = 0;
    }

    unsigned int index = 0;
    unsigned int nb_valid_box_ = 0;
    std::cout<<"preparation time : "<< get_cpu_time() - ts <<" seconds."<<std::endl;
    cpt_iter_ = 0;
    do{
        for (int i=0;i<nb_var_;i++)
        {
            dinput_[i] = Inf(bounds_input_[i]) + mem_index_[i] * eps;
//            std::cout<<dinput_[i]<<" ";
        }
//        std::cout<<std::endl;

        pb_->function(dinput_, doutput_);
        bool inside = true;
        for (int i=0;i<nb_fun_;i++)
        {
            if(doutput_[i] > Sup(bounds_[i]) || doutput_[i] < Inf(bounds_[i]))
            {
                inside = false;
                break;
            }
        }
        if(inside)
        {
            find_one_feasible = true;
            if( optim_crit > doutput_[nb_fun_])
            {
                optim_crit = doutput_[nb_fun_];
                for (int i=0;i<nb_var_;i++)
                    to_store.in[i] = dinput_[i];
                for (int i=0;i<nb_fun_;i++)
                    to_store.out[i] = doutput_[i];
            }
        }

        index = 0;
        while( index < nb_var_  && !( ++mem_index_[index] < nb_steps_[index]) )
        {
            mem_index_[index] = 0;
            index++;
        }
        cpt_iter_++;
    }while(index < nb_var_);



    double te = get_cpu_time();
    std::cout<<"Number of Bissections : "<< cpt_iter_ <<std::endl;
    std::cout<<"Number of valid boxes : "<< nb_valid_box_ <<std::endl;
    std::cout<<"Number of possible boxes : "<< nb_maybe_box_<<std::endl;
    std::cout<<"computation time (wo prep): "<< te - ts <<" seconds."<<std::endl;
    std::cout<<"Time per iteration : "<< (te - ts)/cpt_iter_ <<" seconds."<<std::endl;
    std::cout<<"total time : "<< te - tsglobal <<" seconds."<<std::endl<<std::endl;
    close_files();
    
    if(find_one_feasible)
    {
        
    }
    else
        std::cout<<"no feasible solution found"<<std::endl;
    std::cout<<"crit = "<< optim_crit <<std::endl;
    for (int i=0;i<nb_var_;i++)
        std::cout<<"input["<<i<<"] = "<< to_store.in[i]<<std::endl;
    param_optim out;
    out.nb_bissections = cpt_iter_;
    out.nb_valid_boxes = nb_valid_box_;
    out.nb_possible_boxes = nb_maybe_box_;
    out.computation_time = current_time_ - start_preparation_time_;
    out.computation_time_wo_prep = current_time_ - start_computation_time_;
    out.optim = optim_crit;    
    out.nb_intermediate = 0;
    out.solution_found = nb_valid_box_;    
    return out;        
}
