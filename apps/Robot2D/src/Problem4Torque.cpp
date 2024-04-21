#include "Problem4Torque.h"

void Problem4Torque::init()
{
    nb_var_ = nb_dof_;  // 2 dimensions
    nb_fun_ = 2;  // 2 points

    input_.resize(nb_var_);
    for (int i=0;i<nb_dof_;i++)
    {
	input_[i] = Hull (-1.5,1.5);
// 	input_[i] = Hull (-2,2);
    }

//    input_[0] = Hull (-0.99,-0.98);
//    input_[1] = Hull (1.35,1.40718); //,1.5);



    Rin = 0.0;
    Rout = 0.1;

    bound_.resize(nb_fun_);
    bound_[0] = Power(Hull (Rin,Rout),2);
    bound_[1] = Hull (-0.1,0.1);
//    bound_[2] = Hull (0.0,0.04);

    l = 2.0 / (nb_dof_);  // le bras tendu il peut atteindre deux mètres.

    dframes_.resize(nb_dof_+2);
    Iframes_.resize(nb_dof_+2);
	ICframes_.resize(nb_dof_+2);
	MIframes_.resize(nb_dof_+2);

    dtorque_.resize(nb_dof_+1);
    Itorque_.resize(nb_dof_+1);
	ICtorque_.resize(nb_dof_+1);
	MItorque_.resize(nb_dof_+1);

}

void Problem4Torque::prepare_non_linear( std::vector<MogsInterval>& in)
{
    for (unsigned int i=0;i<nb_dof_;i++)
    {
        in[i].prepare_son(SON_COS_ERROR);
        in[i].prepare_son(SON_SIN_ERROR);
    }
}
