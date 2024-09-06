#include "ChooseSolver.h"
#include "BissectionBasisFunctionSolver.h"
#include "BissectionIntervalSolver.h"
#include "ChooseBasisFunction.h"


void PopotamPrintAllOptions()
{
    ChooseBasisFunction choice_bf;
    
    std::cout<<"\t basis_function : "<<std::endl;
    std::cout<<"\t\t interval "<<std::endl;
    for (int i=0;i<choice_bf.get_nb_basis_type();i++)
    {
        std::cout<<"\t\t "<< choice_bf.get_basis_type(i) <<std::endl;
    }
    std::cout<<"\t bissection_mode : "<<std::endl;
    std::cout<<"\t\t MinFirst "<<std::endl;
    std::cout<<"\t\t MaxFirst "<<std::endl;
    std::cout<<"\t\t Smart "<<std::endl;
}

void PopotamSetSolver(  AbstractCSP* pb,
                        AbstractSolver** solver, 
                        const PopotamOptions* options)
{
    std::string basis_function = "undefined";
    if( ! options->get_string_value("basis_function",basis_function))
    {
        std::cerr<<"Error the basis_function : "<< basis_function<<" is not defined" <<std::endl;
        exit(1);
    }
    
    std::string bissection_type = "undefined";
    if( ! options->get_string_value("bissection_type",bissection_type))
    {
        std::cerr<<"Error the bissection_type : "<< bissection_type<<" is not defined" <<std::endl;
        exit(1);
    }
    
    if (basis_function == "interval")
    {
        *solver = new BissectionIntervalSolver(pb,bissection_type);
    }else
    {
        if (bissection_type == "MinFirst" || bissection_type == "MaxFirst" || bissection_type == "Smart")
        {
            ChooseBasisFunction choice_bf;
            AbstractBasisFunction * bf;
            choice_bf.choose(&bf,basis_function);
            * solver = new BissectionBasisFunctionSolver(pb,bf,bissection_type);
            
        }else
        {
            std::cerr<<"Error the bissection_type : "<< bissection_type<<" is not known" <<std::endl;
            exit(2);            
        }
    }
    
    (*solver)->set_options(options);
}

