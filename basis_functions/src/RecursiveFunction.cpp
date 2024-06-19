#include "RecursiveFunction.h"
#include "utils.h"


void RecursiveFunction::compute_basis_coeff_matrix( const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)
{
    Poly t(0,1);
    assert(inter == Hull(-1,1));
    
    if (order <1)
    {
        std::cerr<<"error in "<< __FILE__<< " at line "<< __LINE__<<" order must be bigger than 1 and is "<<order<<std::endl;
        exit(1);
    }
        
    Poly t1 = 0.5-t*0.5;
    Poly t2 = 0.5+t*0.5;
    Poly ttmp = 1.0;
    
       
    std::vector<Poly> basis(order+1);
    for (int i=0;i<order;i++)    
    {
        basis[i] = ttmp * t1;
        ttmp = ttmp * t2;
    }
    basis[order] = ttmp;
    
    for(int i=0;i<order+1;i++)
        for (int k=0;k<order+1;k++)
            mat(k,i) =  basis[i].getValue(k);

    mat = re_order(mat);
    mat_inverse =mat.inverse();
}
