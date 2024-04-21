#ifndef KRONECKER_H
#define KRONECKER_H

#include "vector"
#include "Interval.h"
#include <list>

#include "LazyCPP.hpp"
#include "Result.h"

typedef double Real;

class Kronecker
{
    public:
        Kronecker( const std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >& mats);
        
        uint get_nb_control_point()const;
        
        double get_pos(uint id, uint p) const;
               
        double get_value(uint lin, uint col) const;
               
        Interval line_product(Eigen::Matrix<double,Eigen::Dynamic,1> & vec);
        
        void prepare_line_product_interval(const std::list<unsigned int > & vec);       
        
        void set_maximum_position(std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> >& max);
        
private:
        
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > basis_matrices_;
        std::vector<uint> size_matrices_;
        std::vector<uint> offsets_;
        uint nb_basis_;
        uint nb_control_points_;
        
        
        Eigen::Matrix<Interval,Eigen::Dynamic,1> line_products_interval_;
        
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> > pos_max_;
        
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> > proba_;
        
        
        std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> > pos_;
};

#endif // KRONECKER_H
