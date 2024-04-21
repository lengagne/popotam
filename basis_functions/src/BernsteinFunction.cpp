#include "BernsteinFunction.h"
#include "utils.h"

void BernsteinFunction::compute_basis_coeff_matrix( const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)
{
//     std::cout<<"BernsteinFunction order = "<< order <<std::endl;
    unsigned int nb_splines =order+1;
    unsigned int size_nodal_vector = nb_splines+order + 1;
    Eigen::Matrix<double,Eigen::Dynamic,1> nodal(size_nodal_vector);
    std::vector<  std::vector<Eigen::Matrix<double,Eigen::Dynamic,1> > > splines(size_nodal_vector);

    /// FIXME consider as constante ?
    double inf = Inf(inter);
    double sup = Sup(inter);
    if (Diam(inter) < 1e-6) sup = inf + 1e-6;
    int cpt = 0;
    for (int i=0;i<order;i++)
            nodal[cpt++] =inf;
    for (int i=0;i<nb_splines -order+1;i++)
            nodal[cpt++] = inf + 1.0* i *( sup - inf) / (nb_splines -order);
    for (int i=0;i<order;i++)
            nodal[cpt++] = sup;


    for(int j=0;j<size_nodal_vector-1;j++)
    {
        splines[j].resize(order+1);
        if (nodal[j] != nodal[j+1])
        {
            splines[j][0] = Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(order+1);
            splines[j][0](0) = 1.;
        }
        else
            splines[j][0] = Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(order+1);
    }

    for (int k=1;k<order+1;k++)
    {
        for(int j=0;j<size_nodal_vector-1-k;j++)
        {
            splines[j][k] = Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(order+1);
            if (nodal[j+k]-nodal[j] >1e-12)
                splines[j][k] = ( offset(splines[j][k-1]) -nodal[j] * splines[j][k-1])/(nodal[j+k]-nodal[j]);

            if (nodal[j+k+1] - nodal[j+1]>1e-12)
                splines[j][k] += ( -offset( splines[j+1][k-1]) +  nodal[j+k+1] * splines[j+1][k-1])/ (nodal[j+k+1] - nodal[j+1]);
        }
    }

    for(int i=0;i<order+1;i++)
    {
        for (int k=0;k<order+1;k++)
        {
            mat(k,i) =  splines[i][order](k);
        }
    }
    
    mat = re_order(mat);
    mat_inverse =mat.inverse();
    
//     std::cout<<"BernsteinFunction fin order = "<< order <<std::endl;
}
