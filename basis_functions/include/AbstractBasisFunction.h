#ifndef AbstractBasisFunction_H
#define AbstractBasisFunction_H
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "Interval.h"
#include <map>

class AbstractBasisFunction
{
    public:
        AbstractBasisFunction()
        {
            mat_order_[1].resize(2,2);
            mat_order_[1](0,0) = 0.5;
            mat_order_[1](0,1) = 0.5;
            mat_order_[1](1,0) = -0.5;
            mat_order_[1](1,1) = 0.5;
            
            mat_inverse_order_[1] = mat_order_[1].inverse();            
        }
        
        ~AbstractBasisFunction()
        {
            mat_order_.clear();
            mat_inverse_order_.clear();
        }
        
    
        void get_basis_coeff_matrix( const Interval& inter,
                                    uint order,
                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
        
        void get_time_max( const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                           Eigen::Matrix<double,Eigen::Dynamic,1>& tmax);
    
    protected:
        
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> re_order(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& M);
        
        virtual void compute_basis_coeff_matrix(    const Interval& inter,
                                                    uint order,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)=0;

    protected:
        std::map< uint, Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > mat_order_;
        std::map< uint, Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > mat_inverse_order_;
        
        uint order_;
        Interval inter_;
};

#endif // AbstractBasisFunction_H
