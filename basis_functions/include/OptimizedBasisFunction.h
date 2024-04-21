#ifndef OptimizedBasisFunction_H
#define OptimizedBasisFunction_H


#ifdef IPOPT_FOUND

#include "AbstractBasisFunction.h"


class OptimizedBasisFunction: public AbstractBasisFunction
{
    public:
       
        OptimizedBasisFunction(const std::string criteria_type="Undefined",bool re_order=false): AbstractBasisFunction()
        {
            criteria_type_ = criteria_type;
        }                    
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);

    protected:
        std::string criteria_type_;
    private:
        
        void rank_matrix( uint order,
                          const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_in,
                          Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_out);
        
        
};

#endif // AbstractBasisFunction_H

#endif //  IPOPT_FOUND
