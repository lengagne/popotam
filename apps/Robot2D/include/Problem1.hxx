
#include <iostream>

template < typename T >
void Problem1::functions(   std::vector<T> & in,
                            std::vector<T> & out,
                            std::vector<Eigen::Matrix<T,3,3> > &  frames)
{
//    std::cout<<"Bissector<T>::functions "<<std::endl;
    frames[0] = Eigen::Matrix<T,3,3>::Identity();
    frames[0](0,0) = 0.;       frames[0](0,1) = -1.;
    frames[0](1,0) = 1.;       frames[0](1,1) = 0.;

    Eigen::Matrix<T,3,3> tmp = Eigen::Matrix<T,3,3>::Identity();
    
    for (int i=0;i<nb_dof_;i++)
    {
        T c = cos(in[i]);
        T s = sin(in[i]);
        tmp(0,0) = c;   tmp(0,1) = -s;  if (i>0) tmp(0,2) = l;
        tmp(1,0) = s;   tmp(1,1) = c;   tmp(1,2) = 0;
        frames[i+1] = frames[i] * tmp;
    }

    tmp = Eigen::Matrix<T,3,3>::Identity();
    tmp(0,2) = l;
    frames[nb_dof_+1] = frames[nb_dof_]*tmp;

    const T x = frames[nb_dof_+1](0,2) ;
    const T y = frames[nb_dof_+1](1,2) ;

    out[0] = x;
    out[1] = y;

    // computation of the COM
    Eigen::Matrix<T,3,1> tmp_com(l/2,0,0), mem_com = Eigen::Matrix<T,3,1>::Zero();
    for (int i=1;i<nb_dof_+2;i++)
        mem_com += frames[i]*tmp_com;

//     mem_com(0).set_name("COM_X");
    out[2] = mem_com(0) / (nb_dof_+1);


    // criteria
    out[3] = 0.0;
    for (int i=0;i<nb_dof_;i++)
        out[3] += in[i]*in[i];
//     for (int i=0;i<3;i++)
// 	std::cout<<"out["<<i<<"] = "<< out[i]<<std::endl;
    // if needed
//     out[0].set_name("Dist");
//     out[1].set_name("COM");

}
