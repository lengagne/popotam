#include "Problem1.h"

void Problem1::init()
{
    nb_var_ = nb_dof_;  // 2 dimensions
    nb_fun_ = 3;  // 2 points

    input_.resize(nb_var_);
    for (int i=0;i<nb_dof_;i++)
    {
        input_[i] = Hull (-1.5,1.5);
    }

//     input_[0] = Hull(0.38,0.39);
//     input_[1] = Hull(-0.90,-0.89);

//     input_[0] = Hull(0.29,0.30);
//     input_[1] = Hull(-0.76,-0.75);
    width = 0.1;

    bound_.resize(nb_fun_);
    bound_[0] = Xt + Hull (-width,width);
    bound_[1] = Yt + Hull (-width,width);
    bound_[2] = Hull (-0.1,0.1);
//    bound_[2] = Hull (0.0,0.04);

    l = 2.0 / (nb_dof_);  // le bras tendu il peut atteindre deux mètres.

    dframes_.resize(nb_dof_+2);
    Iframes_.resize(nb_dof_+2);
	ICframes_.resize(nb_dof_+2);
	MIframes_.resize(nb_dof_+2);

    output_name_.push_back("X");
    output_name_.push_back("Y");
    output_name_.push_back("COM");
    output_name_.push_back("criteria");
}

void Problem1::prepare_non_linear( std::vector<MogsInterval>& in)
{
    for (unsigned int i=0;i<nb_dof_;i++)
    {
        in[i].prepare_son(SON_COS_ERROR);
        in[i].prepare_son(SON_SIN_ERROR);
    }
}
