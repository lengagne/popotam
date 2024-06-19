#ifdef IPOPT_FOUND

#include "OptimizedBasisFunction.h"
#include "BasisFunctionNLP.h"
#include "IpIpoptApplication.hpp"
#include "IpSolveStatistics.hpp"
    
void OptimizedBasisFunction::compute_basis_coeff_matrix(const Interval& inter,
                                                        uint order,
                                                        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)
{
    std::cout<<"We are computing basis function for order"<< order <<" type : "<< criteria_type_<<std::endl;
    
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(order+1,order+1);
#ifdef IPOPT_FOUND
    uint cpt = 0;
    // Create an instance of your nlp...
    SmartPtr < BasisFunctionNLP > mynlp = new BasisFunctionNLP (order,criteria_type_);
    ApplicationReturnStatus status;
    do{
        // Create an instance of the IpoptApplication
        SmartPtr < IpoptApplication > app = IpoptApplicationFactory ();

        // Initialize the IpoptApplication and process the options
        
        status = app->Initialize ();
        if (status != Solve_Succeeded)
        {
            std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<" initialization failed"<< std::endl;
            exit(2);
        }
    
//         app->Options()->SetIntegerValue("print_level", 6);
//         app->Options()->SetStringValue("linear_solver", "ma57");
        app->Options()->SetIntegerValue("max_iter", 10000);
        app->Options()->SetStringValue("hessian_approximation", "limited-memory");
//         app->Options()->SetStringValue("expect_infeasible_problem", "no");
//         app->Options()->SetIntegerValue("print_frequency_iter", 1000);
        app->Options()->SetNumericValue("tol", 1e-12);      
        app->Options()->SetNumericValue("acceptable_tol", 1e-12);      
//         app->Options()->SetNumericValue("bound_relax_factor", 0);      
        
        
        
    //     app->Options()->SetStringValue("derivative_test", "first-order");

        status = app->OptimizeTNLP (mynlp);
        if (status == Solve_Succeeded)
        {
            // Retrieve some statistics about the solve
            Index iter_count = app->Statistics ()->IterationCount ();
            Number final_obj = app->Statistics ()->FinalObjective ();
            std::cout <<"*** The problem solved in " << iter_count << " iterations with final_obj = "<< final_obj << std::endl;
        }   
        
        if(cpt++ > 1000)
        {
            std::cerr<<"Error the optimization failed after 1000 trials"<<std::endl;
            exit(3);
        }
    }while( status != Solve_Succeeded);

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> mat_init;
    mynlp->get_matrix(mat_init);
    rank_matrix(order,mat_init,mat);
    mat = re_order(mat);
    mat_inverse = mat.inverse();
    
#else
    std::cerr<<"You cannot use this class, since IPOPT was not found."<<std::endl;
    exit(1);
    M = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>::Identity(order+1,order+1);
    mat = M;
    mat = re_order(mat);
    mat_inverse = M;
#endif
}

void OptimizedBasisFunction::rank_matrix(   uint order,
                                            const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_in,
                                            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_out)
{
    uint nb = order+1;
//     std::cout<<"nb = "<< nb <<std::endl;
    mat_out.resize(nb,nb);
    std::vector<uint> max_index(nb);
    
    // we get the max for each function
    uint nb_time = nb*10 +1;
    for(int i=0;i<nb;i++)
    {
        max_index[i] = 0;
        double max = 0.;
        
        for (int j=0;j<nb_time;j++)
        {
            double t = -1.0 + (2.0*j/(nb_time-1));
            
            double val = 0;
            for (int k=0;k<nb;k++)
                val+= pow(t,k) * mat_in(k,i);
            
            if (val > max)
            {
                max = val;
                max_index[i] = j;
            }
        }
    }
    
    uint cpt =0;
    for (int j=0;j<nb_time;j++) for (int i=0;i<nb;i++)  if(j==max_index[i])
    {
        for (int k=0;k<nb;k++)
            mat_out(k,cpt) = mat_in(k,i);
        cpt++;
    }
}

#endif // IPOPT_FOUND
