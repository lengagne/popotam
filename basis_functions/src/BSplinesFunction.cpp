#include "BSplinesFunction.h"
#include "utils.h"

Poly BSplinesFunction::w( uint i, uint k, const Poly& x) const
{
    return ( nodal_vector_[i]-x) / (nodal_vector_[i]-nodal_vector_[i+k]);
}

Poly BSplinesFunction::B( uint i, uint k, const Poly& x) const
{
    if(k==0)
    {
        if (  nodal_vector_[i] <= 0. && 0. < nodal_vector_[i+1])
            return 1.0;
        return 0.0;
    }
    return w(i,k,x)*B(i,k-1,x)- (w(i+1,k,x)-1.0)*B(i+1,k-1,x);
}

void BSplinesFunction::compute_basis_coeff_matrix( const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)
{
    Poly t(0,1);
    assert(inter == Hull(-1,1));
    
    unsigned int nb_splines =order+1;
    nodal_vector_.resize(2*order+2);

    uint cpt=0;
    for (int i=0;i<order+1;i++)
        nodal_vector_[cpt++] = -1.0 -(order-i)*2;
    
    for (int i=0;i<order+1;i++)
        nodal_vector_[cpt++] = (i)*2 + 1.0 ;
    
    std::vector<Poly> basis(nb_splines);
        

    for (int i=0;i<nb_splines;i++)    
        basis[i] = B(i,order,t);    

    for(int i=0;i<order+1;i++)
        for (int k=0;k<order+1;k++)
            mat(k,i) =  basis[i].getValue(k);

    mat = re_order(mat);
    mat_inverse =mat.inverse();
}
