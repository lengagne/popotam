#ifndef BernsteinFunction_H
#define BernsteinFunction_H

#include "AbstractBasisFunction.h"


class BernsteinFunction: public AbstractBasisFunction
{
    public:
       
        BernsteinFunction(): AbstractBasisFunction()
        {

        }           
        
        ~BernsteinFunction()
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
