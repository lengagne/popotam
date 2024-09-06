#include <iostream>
#include <vector>

#include "ChooseSolver.h"
#include "Problem1.h"
#include "Problem4.h"
#include "Problem7.h"

#include "Problem1Torque.h"
#include "Problem4Torque.h"
#include "Problem7Torque.h"

#include "construct_filename_2D.h"



int main( int argc, char** argv)
{
    std::cout<<"type_of_problem : 2D"<<std::endl;

    if (argc ==1 ||  std::string( argv[1]) == "h" || std::string( argv[1]) =="help")
    {
        std::cout<<"Call ./Robot2D --ndof=XX --problem=YY --popotam-options"<<std::endl;
        PopotamPrintAllOptions();
    }    
        
    PopotamOptions *options = new PopotamOptions();
//     options->add_type_int("ndof");
//     options->add_type_int("problem");
    
    options->read(argc,argv);
    
	int ndof = 2;
     if(!options->get_int_value("ndof",ndof))
     {
        std::cerr<<"You must specify an option --ndof=XX"<<std::endl;
        exit(101);
     }
    
    int problem = 1;
    if(!options->get_int_value("problem",problem))
    {
        std::cerr<<"You must specify an option --problem=XX"<<std::endl;
        exit(102);
     }

    std::cout<<"ndof = "<< ndof <<std::endl;
    std::cout<<"problem = "<< problem <<std::endl;

	bool print=false;;
    AbstractCSP* pb;
    switch(problem)
    {
        case(1): pb = new Problem1(ndof,0.2,1.7);   break;
        case(2): pb = new Problem1(ndof,0.4,1.4);   break;
        case(3): pb = new Problem1(ndof,0.6,1.1);   break;
        case(4): pb = new Problem4(ndof,0.2,1.7);   break;
        case(5): pb = new Problem4(ndof,0.4,1.4);   break;
        case(6): pb = new Problem4(ndof,0.6,1.1);   break;
        case(7): pb = new Problem7(ndof,0.2,1.7);   break;
        case(8): pb = new Problem7(ndof,0.4,1.4);   break;
        case(9): pb = new Problem7(ndof,0.6,1.1);   break;

        case(11): pb = new Problem1Torque(ndof,0.2,1.7);   break;
        case(12): pb = new Problem1Torque(ndof,0.4,1.4);   break;
        case(13): pb = new Problem1Torque(ndof,0.6,1.1);   break;
        case(14): pb = new Problem4Torque(ndof,0.2,1.7);   break;
        case(15): pb = new Problem4Torque(ndof,0.4,1.4);   break;
        case(16): pb = new Problem4Torque(ndof,0.6,1.1);   break;
        case(17): pb = new Problem7Torque(ndof,0.2,1.7);   break;
        case(18): pb = new Problem7Torque(ndof,0.4,1.4);   break;
        case(19): pb = new Problem7Torque(ndof,0.6,1.1);   break;	
        default: std::cout<<"Problem not found"<<std::endl; exit(0);
    }
    
    
    
    AbstractSolver* solver;
    PopotamSetSolver( pb, &solver, options);
    solver->solve();
        
    delete pb;
    delete solver;
    
    return 0;
}
