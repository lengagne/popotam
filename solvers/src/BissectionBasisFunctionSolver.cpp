#include "BissectionBasisFunctionSolver.h"
#include <string.h>
#include <time.h>


BissectionBasisFunctionSolver::BissectionBasisFunctionSolver(AbstractCSP* pb, 
                                                             AbstractBasisFunction*bf,
                                                            const std::string& bissection_type
                                                            )
{
    LazyReset();
    bf_ = bf;
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    
    if( bissection_type=="MinFirst")
        bissection_type_ = 0;
    else if( bissection_type=="MaxFirst")
        bissection_type_ = 1;
    else if( bissection_type=="Smart")
        bissection_type_ = 2;    
    else
    {
        std::cerr<<"ERROR in "<< __FILE__<<" at line "<< __LINE__<<" the bissection type "<< bissection_type <<" is not defined"<<std::endl;
        exit(12);
    }    
}

BissectionBasisFunctionSolver::~BissectionBasisFunctionSolver()
{
    close_files();
    results.clear();
}

void BissectionBasisFunctionSolver::evaluate(   const std::vector<Interval> &in,
                                                std::vector<Interval> &out)
{
    solve_optim_ = true;
    init(1e-3);
    out.resize(nb_fun_);
    Result tmp;
    tmp.in = in;   
    tmp.out.resize(nb_fun_);

    current_evaluator_->evaluate( tmp);
    out = tmp.out;
    
// //     current_value_ = current_vector_.back();
//     current_vector_.pop_back();         
//     
//     
//     
//     set_current_value();
 /*   
    for (int i=0;i<nb_fun_;i++)
    {
        compute_intermediate_for(i);
        out[i] = infos[i]->update_from_inputs();
    }
    if(solve_optim_)
    {
        compute_intermediate_for(nb_fun_);
//         std::cout<<"calcul de la deniere "<<std::endl;
        out[nb_fun_] = info_crit_->update_from_inputs();
    }else
    {
        std::cout<<"on ne calcule pas le dernier"<<std::endl;
    }*/
}

void BissectionBasisFunctionSolver::init(double eps)
{
    if (init_done)  return;
    BasisFunctionSolver::init(eps);
    
    bounds_input_ = pb_->get_input();
    bounds_ = pb_->get_bound();
    
    current_evaluator_ = new EvalCurrent(pb_,bf_);

//     infos.resize(nb_fun_);
//     for (int i=0;i<nb_fun_;i++)
//     {
//         infos[i] = new IntervalEstimator(bf_);
//     }
// 
//     if(solve_optim_)
//     {
//         info_crit_ = new IntervalEstimator( bf_);
//     }
    
    current_evaluator_->init();
    
    BasisFunctionSolver::init_end();
}

