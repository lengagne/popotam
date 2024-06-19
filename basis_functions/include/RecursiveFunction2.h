#ifndef RecursiveFunction2_H
#define RecursiveFunction2_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class RecursiveFunction2: public AbstractBasisFunction
{
    public:
       
        RecursiveFunction2(): AbstractBasisFunction()
        {

        }         
        
        ~RecursiveFunction2()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
        
        std::string get_name() const
        {
            return "Recursive2";
        }
    protected:
    private:
};

#endif // AbstractBasisFunction_H
