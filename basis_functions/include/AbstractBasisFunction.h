#ifndef AbstractBasisFunction_H
#define AbstractBasisFunction_H
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "Interval.h"
#include <map>

#define BS_THRESHOLD  	std::numeric_limits<double>::epsilon()

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
        
        // function used in case of numerically given values (0 is not really 0)
        void check_matrix_quasi_null_coeff( Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
    
        void get_basis_coeff_matrix( const Interval& inter,
                                    uint order,
                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
        
        void get_time_max( const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                           Eigen::Matrix<double,Eigen::Dynamic,1>& tmax);
        
        virtual std::string get_name() const = 0;
    
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
