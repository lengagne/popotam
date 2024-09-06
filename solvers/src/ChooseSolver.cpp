#include "ChooseSolver.h"
#include "BissectionBasisFunctionSolver.h"
#include "BissectionIntervalSolver.h"
#include "RandomSolver.h"
#include "ChooseBasisFunction.h"


void PopotamPrintAllOptions()
{
    ChooseBasisFunction choice_bf;
    
    std::cout<<"\t --type_optim : "<<std::endl;
    std::cout<<"\t\t Interval "<<std::endl;
    std::cout<<"\t\t Random "<<std::endl;
    std::cout<<"\t\t\t --nb_random=(int) "<<std::endl;
    
    std::cout<<"\t\t BasisFunction "<<std::endl;

    std::cout<<"\t\t\t --basis_function : "<<std::endl;
    for (int i=0;i<choice_bf.get_nb_basis_type();i++)
    {
        std::cout<<"\t\t\t\t "<< choice_bf.get_basis_type(i) <<std::endl;
    }
    std::cout<<"\t --bissection_mode : "<<std::endl;
    std::cout<<"\t\t MinFirst "<<std::endl;
    std::cout<<"\t\t MaxFirst "<<std::endl;
    std::cout<<"\t\t Smart "<<std::endl;
    
    std::cout<<"\t --precision=(double) "<<std::endl;    
    
}

void PopotamSetSolver(  AbstractCSP* pb,
                        AbstractSolver** solver, 
                        const PopotamOptions* options)
{
    std::string type_optim = "undefined";
    if( ! options->get_string_value("type_optim",type_optim))
    {
        std::cerr<<"Error the type_optim : "<< type_optim<<" is not defined" <<std::endl;
        exit(1);
    }
    
    if (type_optim == "Interval")
    {
        *solver = new BissectionIntervalSolver(pb,options);
    }else if (type_optim == "Random")
    {
        *solver = new RandomSolver(pb,options);
    }else if (type_optim == "BasisFunction")
    {
        * solver = new BissectionBasisFunctionSolver(pb,options);
    }
}

