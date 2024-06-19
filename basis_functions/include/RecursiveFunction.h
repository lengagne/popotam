#ifndef RecursiveFunction_H
#define RecursiveFunction_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class RecursiveFunction: public AbstractBasisFunction
{
    public:
       
        RecursiveFunction(): AbstractBasisFunction()
        {

        }         
        
        ~RecursiveFunction()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
        
        std::string get_name() const
        {
            return "Recursive";
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H
