#include "utils.h"

double get_cpu_time(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + 1e-6 * t.tv_usec;
//    return (double)clock() / CLOCKS_PER_SEC;
}

Eigen::Matrix<double,Eigen::Dynamic,1> offset( const  Eigen::Matrix<double,Eigen::Dynamic,1>& in)
{
    unsigned int s = in.size();
    Eigen::Matrix<double,Eigen::Dynamic,1> out(s);
    out(0) = 0;
    for(int i=0;i<s-1;i++)
        out(i+1) = in(i);
    return out;
}

double random_init()
{
    return (double(rand())/double((RAND_MAX)) * 2.0)-1.0; 
}

bool recursive_kronecker(const std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >& mats,
                                                                const Eigen::Matrix<double,Eigen::Dynamic,1> &vec,
                                                                Eigen::Matrix<double,Eigen::Dynamic,1> & out,
                                                                unsigned int index)
{
    if (index == mats.size()-1)
    {
        if (vec == Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(vec.size()))
        {
            out =  Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(vec.size());
            return false;
        }
        out = mats[index] * vec;
        return true;
    }

    const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&m = mats[index];
    unsigned int size_mat = m.rows();
    unsigned int size_vec = vec.size();
    unsigned int offset = size_vec / size_mat;
    Eigen::Matrix<double,Eigen::Dynamic,1> tmp_vec(offset);
    out = Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(size_vec);

    bool computation = false;
    for (int i=0;i<size_mat;i++)
    {
        bool segment_is_non_null = recursive_kronecker(mats, vec.segment(i*offset,offset),tmp_vec ,index+1 );
        if(segment_is_non_null)
        {
            for (int j=0;j<size_mat;j++)
            {
                out.segment(j*offset,offset) += m(j,i) * tmp_vec;
            }
            computation = true;
        }
    }
    return computation;
}

std::string ToString( const double& a)
{
     std::ostringstream ss1;
     ss1 << a;
     return ss1.str();
}

double MIN(double a,double b)
{
        if (b<a)
            return b;
        return a;
}

double MAX(double a,double b)
{
        if (b>a)
            return b;
        return a;
}
