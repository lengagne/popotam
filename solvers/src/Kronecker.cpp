#include "Kronecker.h"
#include <Eigen/Sparse>
#include <unsupported/Eigen/KroneckerProduct>

Kronecker::Kronecker( const std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >& mats)
{
    basis_matrices_ = mats;
    nb_basis_ = basis_matrices_.size();
    size_matrices_.resize(nb_basis_);
    for (int i=0;i<nb_basis_;i++)
    {
        uint s = mats[i].rows();
        size_matrices_[i] = s;
    }
    offsets_.resize(nb_basis_);
    for (int i=0;i<nb_basis_;i++)
    {   
        uint off = 1;
        for (int j=i+1;j<nb_basis_;j++)
            off *= size_matrices_[j];                
        offsets_[i] = off;
    }
    
    nb_control_points_ = 1;
    for (int i=0;i<nb_basis_;i++)
        nb_control_points_ *= size_matrices_[i];
}

uint Kronecker::get_nb_control_point()const
{
    return nb_control_points_;
}

double Kronecker::get_pos(uint id, uint p) const
{
    return  pos_max_[id]((p/offsets_[id])%size_matrices_[id]);
}

double Kronecker::get_value(uint lin, uint col) const
{
    double out = 1;
    for (int i=0;i<nb_basis_;i++)
    {
        out *= basis_matrices_[i]((lin/offsets_[i])%size_matrices_[i], (col/offsets_[i])%size_matrices_[i]);
    }
    return out;
}

Interval Kronecker::line_product(Eigen::Matrix<double,Eigen::Dynamic,1> & vec)
{
    if (vec.size() != line_products_interval_.size())
    {
        std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<" : size not aligned"<<std::endl;
        std::cout<<"vec.size() = "<< vec.size()<<std::endl;
        std::cout<<"line_products_interval_.size() = "<< line_products_interval_.size()<<std::endl;
        exit(1);
    }    
    uint nb = vec.size();
    Interval out = 0;
    for (int i=0;i<nb;i++)
    {
        out+= line_products_interval_(i)*vec(i);
    }

    return out;    
}

void Kronecker::prepare_line_product_interval(const std::list<unsigned int > & vec)
{
    uint s = vec.size();
    line_products_interval_.resize(s);
    uint col = 0;
    for(std::list<unsigned int >::const_iterator it = vec.begin(); it != vec.end(); it++)
    {        
        line_products_interval_(col) = get_value(0,*it);
        col ++;
    }
    
    col = 0;
    for(std::list<unsigned int >::const_iterator it = vec.begin(); it != vec.end(); it++)
    {
        for (int j=1;j<nb_control_points_;j++)
        {
            line_products_interval_(col) = Hull( line_products_interval_(col), get_value(j,*it));
            
        }
        col++;
    }
}

void Kronecker::set_maximum_position(std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> >& max)
{
    pos_max_ = max;
//     for (int i=0;i<pos_max_.size();i++)
//         std::cout<<"pos_max_("<<i<<") = "<< pos_max_[i].transpose()<<std::endl;
    
    pos_.resize(nb_control_points_);   
    for (int i=0;i<nb_control_points_;i++)
    {
        std::vector<bool> tmp_inf;
        pos_[i].resize(nb_basis_);
        for (int j=0;j<nb_basis_;j++)
        {
            pos_[i](j) = get_pos(j,i);
        }
//         std::cout<<"pos_["<<i<<"] = "<< pos_[i].transpose()<<std::endl;
        
        double dmin = 10.0;
        uint cpt = 0;
        Eigen::Matrix<double,Eigen::Dynamic,1> tmp, pmax = (1.0/nb_basis_)* Eigen::Matrix<double,Eigen::Dynamic,1>::Ones(nb_basis_);
        // calcul de distance
        for (int k=0;k<i;k++)
        {
            tmp = pos_[i] - pos_[k];
            double d = (tmp.norm());
            if (d == dmin)
            {
                pmax += tmp;
                cpt ++;
            }
            if (d< dmin)
            {
                dmin = d;
                pmax = tmp;
                cpt = 1;
            }
        }
        if (cpt)
            pmax /= cpt;
        
        pmax += Eigen::Matrix<double,Eigen::Dynamic,1>::Ones(nb_basis_) * 10.0;
        pmax.normalize();
        
        proba_.push_back(pmax);        
    }
//     for (int i=0;i<nb_control_points_;i++)
//         std::cout<<"proba_["<<i<<"] = "<< proba_[i].transpose()<<std::endl;
}
