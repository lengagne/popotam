#ifndef BSplinesFunction_H
#define BSplinesFunction_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class BSplinesFunction: public AbstractBasisFunction
{
    public:
       
        BSplinesFunction(): AbstractBasisFunction()
        {

        }         
        
        ~BSplinesFunction()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);
        std::string get_name() const
        {
            return "BSplines";
        }

    protected:
    private:
        Poly w( uint i, uint k, const Poly& x) const;
        Poly B( uint i, uint k, const Poly& x) const;
        
        std::vector<double> nodal_vector_;
};

#endif // AbstractBasisFunction_H
