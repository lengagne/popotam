#include "RandomSolver.h"
#include <string.h>

double random(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

RandomSolver::RandomSolver( AbstractCSP* pb,
                            const PopotamOptions *options)
{
    options_ = options;
    
    if( ! options_->get_int_value("nb_random",nb_random_))
    {
        std::cerr<<"Error the nb_random is not defined" <<std::endl;
        exit(1);
    }    
    
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    std::cout<<"Creating RandomSolver for problem : "<< pb_->get_problem_name()<<std::endl;
}

RandomSolver::~RandomSolver()
{
    close_files();
    results.clear();
}

void RandomSolver::evaluate(const std::vector<Interval> &in,
                                    std::vector<Interval> &out)
{
    std::cerr<<"Function evaluate not defined yet for RandomSolver"<<std::endl;

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
    for (int k=0;k<nb_random_;k++)
    {
        for (int i=0;i<nb_var_;i++)
            input[i] = random(Inf(in[i]), Sup(in[i]));

        pb_->function(input,output);
        for (int i=0;i<nb_fun_;i++)
            out[i] = Hull(out[i], output[i]);
    }
}



param_optim RandomSolver::solve_optim(double eps)
{
    optim_info_.optim_crit_ = std::numeric_limits<double>::max();
    optim_info_.find_one_feasible_ =false;
    std::cout<<"RandomSolver::solve_csp"<<std::endl;
    double ts = get_cpu_time();
    double tsglobal = ts;

    bounds_ = pb_->get_bound();
    bounds_input_ = pb_->get_input();

    std::vector<double> dinput_(nb_var_), doutput_(nb_fun_+ pb_->get_criteria());
    Result current(pb_->get_input(), nb_fun_, pb_->get_criteria());
    optim_info_.optim_ = current;
    
    std::cout<<"preparation time : "<< get_cpu_time() - ts <<" seconds."<<std::endl;
    cpt_iter_ = 0;
    do{
        for (int i=0;i<nb_var_;i++)
        {
            dinput_[i] = random( Inf(bounds_input_[i]), Sup(bounds_input_[i]));
        }
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
        if(pb_->get_criteria() && inside)
        {
            optim_info_.find_one_feasible_ = true;
            if( optim_info_.optim_crit_ > doutput_[nb_fun_])
            {
                optim_info_.optim_crit_ = doutput_[nb_fun_];
                for (int i=0;i<nb_var_;i++)
                    optim_info_.optim_.in[i] = dinput_[i];
                for (int i=0;i<nb_fun_+pb_->get_criteria();i++)
                    optim_info_.optim_.out[i] = doutput_[i];
            }
        }
        cpt_iter_++;
    }while(cpt_iter_ < nb_random_);

    return set_results();
}
