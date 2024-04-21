#include "Problem7.h"

void Problem7::init()
{
    nb_var_ = nb_dof_;  // 2 dimensions
    nb_fun_ = 2;  // 2 points

    input_.resize(nb_var_);
    for (int i=0;i<nb_dof_;i++)
    {
// 	input_[i] = Hull (-2,2);
	input_[i] = Hull (-1.5,1.5);
    }


//    Xt = 0.2;
//    Yt = 1.7;
    Rin = 0.05;
    Rout = 0.15;

    bound_.resize(nb_fun_);
    bound_[0] = Power(Hull (Rin,Rout),2);
    bound_[1] = Hull (-0.1,0.1);

    l = 2.0 / (nb_dof_);  // le bras tendu il peut atteindre deux mètres.

    dframes_.resize(nb_dof_+2);
    Iframes_.resize(nb_dof_+2);
	ICframes_.resize(nb_dof_+2);
	MIframes_.resize(nb_dof_+2);
}

void Problem7::prepare_non_linear( std::vector<MogsInterval>& in)
{
    for (unsigned int i=0;i<nb_dof_;i++)
    {
        in[i].prepare_son(SON_COS_ERROR);
        in[i].prepare_son(SON_SIN_ERROR);
    }
}
