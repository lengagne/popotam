#include "AbstractBasisFunction.h"
       
void AbstractBasisFunction::get_basis_coeff_matrix( const Interval& inter,
                                                    uint order,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse
                                                  )
{
    inter_ = inter;
    std::map<uint, Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >::iterator it ;
    
//     std::cout<<"looking for order "<< order <<" if already asked. "<<std::endl;
    it = mat_order_.find(order);      
    if(it == mat_order_.end())
    {
        compute_basis_coeff_matrix(inter, order,mat,mat_inverse);        
        mat_order_[order] = mat;
        mat_inverse_order_[order] = mat_inverse;
    }
    else
    {
        mat = mat_order_[order];
        mat_inverse = mat_inverse_order_[order];
    }
    
    
    
}

void AbstractBasisFunction::get_time_max( const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                          Eigen::Matrix<double,Eigen::Dynamic,1>& tmax)
{
    uint s = mat.rows();
    tmax.resize(s);      
    double inf = -1.0; //Inf(inter_);
    double diam = 2.0; //Diam (inter_);
    
    int nb = 101;
    double delta = diam / (nb-1);
//     std::cout<<"mat = "<< mat <<std::endl;
//     std::cout<<"s = "<< s <<std::endl;
    for (int i=0;i<s;i++)
    {
        // compute each value
        double max = 0;
        double maxt = inf;
        for (int j=0;j<nb;j++)
        {
            double t = inf + j *delta;
//             std::cout<<"t = "<< t << std::endl;
            double val = 0;
            for (int k=0;k<s;k++)
                val +=  mat(k,i) * pow (t,k);
//             std::cout<<"val = "<< val << std::endl;
            if (val > max)
            {
                max = val;
                maxt = t;
            }          
        }
        tmax(i) = maxt;
//         std::cout<<"tmax("<<i<<") = "<< tmax(i) << "  ("<< max <<")"<<std::endl;
    }
//     std::cout<<"tmax = "<< tmax.transpose() <<std::endl;
    
}

Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> AbstractBasisFunction::re_order(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& M)
{
//     std::cout<<"Re order matrice"<<std::endl;
    Eigen::Matrix<double,Eigen::Dynamic,1> time_max;
    get_time_max(M,time_max);

    uint nb = M.rows();
    // on rempli le vector avec les min et les maxt
    std::vector<int> iter_order(nb);
    std::vector<bool> done(nb);
    for (int i=0;i<nb;i++)
        done[i] = false;
    
    for (int i=0;i<nb;i++)
    {
        double m = 2;
        for (int j=0;j<nb;j++) if (!done[j])
        {
            if (time_max(j) < m)
            {
                m = time_max(j);
                iter_order[i] = j;
            }
        }
        done[iter_order[i]] = true;
//         std::cout<<"iter_order("<<i<<") = "<< iter_order[i] <<std::endl;
    }
    
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> mat(nb,nb);
    for (int i=0;i<nb;i++)
    {
        uint index = iter_order[i];
        for (int j=0;j<nb;j++)
        {
            mat(j,i) = M(j,index);
        }
    }
    
    return mat;
}
