#ifndef RecursiveBSplinesFunction2_H
#define RecursiveBSplinesFunction2_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class RecursiveBSplinesFunction2: public AbstractBasisFunction
{
    public:
       
        RecursiveBSplinesFunction2(): AbstractBasisFunction()
        {

        }         
        
        ~RecursiveBSplinesFunction2()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);

    protected:
    private:
};

#endif // AbstractBasisFunction_H
