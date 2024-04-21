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
        std::cout<<"Call ./Robot2D ndof problem basis_function bissection_mode precision"<<std::endl;
        PopotamPrintAllOptions();
        exit(0);
    }    
        
    SolverOptions options;
    
	unsigned int ndof = 2;
    unsigned int npb = 1;
    double precision = 0.001;
    uint bissection_mode = 0;
    int test = -1;
	if(argc > 1)
        ndof = atoi(argv[1]);
	if(argc > 2)
        npb = atoi(argv[2]);    
	if(argc > 3)
    {
        options.add_string_option ( "basis_function", argv[3]);
    }        
	if(argc > 4)
    {
        options.add_string_option ( "bissection_type", argv[4]);
    }
	if(argc > 5)
    {
        options.add_double_option ( "precision", atof(argv[5]));
    }
    
    
    std::string save_filename;    
    if(test != -1)
    {
        //FIXME
        if (argc >= 7)
        {
            save_filename = argv[6];
        }else
        {
            save_filename =  construct_filename_2D(ndof,npb,precision,bissection_mode,test);
        }
        options.add_string_option ( "save_filename", save_filename);
    }
    
    
    std::cout<<"******************************" <<std::endl;
    std::cout<<"ndof = "<< ndof <<std::endl;
    std::cout<<"npb = "<< npb <<std::endl;
    std::cout<<"precision = "<< precision <<std::endl;

	bool print=false;;
    AbstractCSP* pb;
    switch(npb)
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
