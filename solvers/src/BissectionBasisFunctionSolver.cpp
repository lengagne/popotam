#include "BissectionBasisFunctionSolver.h"
#include <string.h>
#include <time.h>
#include "ChooseBasisFunction.h"

BissectionBasisFunctionSolver::BissectionBasisFunctionSolver(AbstractCSP* pb,                                                              
                                                             const PopotamOptions *options)
{
    LazyReset();

    std::string basis_function = "undefined";
    if( ! options->get_string_value("basis_function",basis_function))
    {
        std::cerr<<"Error the basis_function : "<< basis_function<<" is not defined" <<std::endl;
        exit(1);
    }    
    
    ChooseBasisFunction choice_bf;
    choice_bf.choose(&bf_,basis_function);

    
    options_ = options;
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    
    std::string bissection_type ="undefined";
    if( ! options_->get_string_value("bissection_type",bissection_type))
    {
        std::cerr<<"Error the bissection_type : "<< bissection_type<<" is not defined" <<std::endl;
        exit(1);
    }    
    
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
    
}

void BissectionBasisFunctionSolver::init(double eps)
{
    if (init_done)  return;
    BasisFunctionSolver::init(eps);
    
    bounds_input_ = pb_->get_input();
    bounds_ = pb_->get_bound();
    
    current_evaluator_ = new EvalCurrent(pb_,bf_);

    current_evaluator_->init();
    
    BasisFunctionSolver::init_end();
}

