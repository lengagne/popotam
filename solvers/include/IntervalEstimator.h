#ifndef __INTERVALESTIMATOR_H__
#define __INTERVALESTIMATOR_H__

#include "MogsInterval.h"
#include "utils.h"
#include "Kronecker.h"
#include "AbstractBasisFunction.h"
#include "LazyCPP.hpp"
#include "Result.h"
#include <limits>

// #define OFFSET_ERROR_INDEX std::numeric_limits<int>::max()/2
#define OFFSET_ERROR_TH 1e-9

/// test if the Interval in is INSIDE/OUTSIDE or OVERLAPPING the bound
check_constraint test_Interval( const Interval &in ,
                               const Interval & bound);


class IntervalEstimator{
    public:
        IntervalEstimator(AbstractBasisFunction* bf, bool avoid_error = false);
        
//         ~IntervalEstimator(){};

        unsigned int get_index(mem*m ) const;

        unsigned int get_index_input(mem* m) const;

        virtual unsigned int  prepare_coeffs( const MogsInterval& out, unsigned int num_out);

        virtual Interval update_from_inputs( );
//         virtual check_constraint update_from_inputs( Interval& out, Interval& bound);
        virtual check_constraint update_from_inputs( Result& res, 
                                                     Interval& bound,
                                                     uint index_ctr);
        
        virtual std::list<unsigned int> get_dep_intermediate()const
        {
            return depend_intermediate_;
        }

    unsigned int nb_in_;
    std::vector<MogsInterval*> dep_;
    std::vector<unsigned int> order_,coefdep_;
    std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_;
    std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_inverse_;
    
    std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> > pos_max_;

    std::vector<MogsInterval*> dep_inputs_;
    std::vector<unsigned int> order_inputs_,coefdep_inputs_;
    std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > local_M_inverse_inputs_;

    std::vector<mem*> all_dependancies_;
    
    std::list< unsigned int> depend_intermediate_;

    Kronecker* kron_solver_inputs_;
    Kronecker* kron_solver_errors_;

//     Eigen::Matrix<LazyVariable,Eigen::Dynamic,1> sparse_coeff_inputs_;
    Eigen::Matrix<Real,Eigen::Dynamic,1> sparse_coeff_errors_;
//    Eigen::Matrix<unsigned int,Eigen::Dynamic,1> sparse_index_inputs_, sparse_index_errors_;
//     unsigned int nb_sparse_inputs_;
    unsigned int nb_sparse_errors_;

    unsigned int nb_control_point_inputs_;
    unsigned int nb_control_point_errors_;


    unsigned int num_out_;
    bool avoid_error_computation_ = false;
    // use to compute the coefficients of the basis functions
    AbstractBasisFunction* bf_;       
    
//     std::vector< std::vector<double> > cut_direction_;
//     std::vector< std::vector< bool > > cut_inf_;

};


#endif // OUTPUT_INFOS_H
