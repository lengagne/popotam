//      Copyright (C) 2012 lengagne (lengagne@gmail.com)
// 
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
// 
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
// 
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//      This program was developped in the following labs:
//      from 2022 : Université Blaise Pascal / axis : ISPR / theme MACCS

#ifdef IPOPT_FOUND

#include "BasisFunctionNLP.h"
#include <cassert>
#include <iomanip>      // std::setprecision
#include <fstream>

using namespace Ipopt;

/* Constructor. */
BasisFunctionNLP::BasisFunctionNLP (uint order, const std::string& criteria_type)
{
    criteria_type_ = criteria_type;
    order_ = order;
    double_pb_ = new BasisFunctionProblem<double>(order,criteria_type);
    Fdouble_pb_ = new BasisFunctionProblem<AD>(order,criteria_type);
}

BasisFunctionNLP::~BasisFunctionNLP ()
{
}

bool BasisFunctionNLP::get_nlp_info (Index & n, Index & m, Index & nnz_jac_g,
		     Index & nnz_h_lag, IndexStyleEnum & index_style)
{
//     std::cout<<"get_nlp_info"<<std::endl;
	// The problem described in BasisFunctionNLP.hpp has 2 variables, x1, & x2,
	nb_param_ = double_pb_->GetNumberParam();
//     std::cout<<"on considère "<< nb_param_ << " paramètres "<<std::endl;
    
    if (nb_param_ != Fdouble_pb_->GetNumberParam())
    {
        std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<std::endl;
        std::cerr<<"The two values must be equals"<<std::endl;
        exit(1);
    }
    
    n= nb_param_;
    x_.resize(nb_param_);
    Fx_.resize(nb_param_);
    
	// one equality constraint,
	m = double_pb_->GetNumberConstraints();
    if (m != Fdouble_pb_->GetNumberConstraints())
    {
        std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<std::endl;
        std::cerr<<"The two values must be equals"<<std::endl;
        exit(1);
    }

	// 2 nonzeros in the jacobian (one for x1, and one for x2),
	nnz_jac_g = n*m;

	// and 2 nonzeros in the hessian of the lagrangian
	// (one in the hessian of the objective for x2,
	//  and one in the hessian of the constraints for x1)
	nnz_h_lag = 0;

	// We use the standard fortran index style for row/col entries
	index_style = C_STYLE;
//     std::cout<<"get_nlp_info done"<<std::endl;
	return true;
}

bool BasisFunctionNLP::get_bounds_info (Index n, Number * x_l, Number * x_u,
			Index m, Number * g_l, Number * g_u)
{
//     std::cout<<"get_bounds_info"<<std::endl;
	// here, the n and m we gave IPOPT in get_nlp_info are passed back to us.
	// If desired, we could assert to make sure they are what we think they are.
    std::vector<double> low,high;
    double_pb_->GetParamLimits(low,high);
    for (int i=0;i<n;i++)
    {
       x_l[i] = low[i]; 
       x_u[i] = high[i];
    }
		
	// we have one equality constraint, so we set the bounds on this constraint
	// to be equal (and zero).
	double_pb_->GetConstraintLimits(low,high);
    for (int i=0;i<m;i++)
    {
        g_l[i] = low[i];
        g_u[i] = high[i];
    }
    
    return true;
}

bool BasisFunctionNLP::get_starting_point (Index n, bool init_x, Number * x,
			   bool init_z, Number * z_L, Number * z_U,
			   Index m, bool init_lambda, Number * lambda)
{
	// Here, we assume we only have starting values for x, if you code
	// your own NLP, you can provide starting values for the others if
	// you wish.
	assert (init_x == true);
	assert (init_z == false);
	assert (init_lambda == false);
        
    std::vector<double> xinit(n);
    std::vector<AD> ad_xinit(n);

	// we initialize x in bounds, in the upper right quadrant
    for (int i=0;i<n;i++)
        x[i] = random_init();
//         x[i] = -1.0 + 2.*i/n;
    
	return true;
}

bool BasisFunctionNLP::eval_f (Index n, const Number * x, bool new_x, Number & obj_value)
{
//     std::cout<<"eval_f"<<std::endl;
    for (int i=0;i<nb_param_;i++)
        x_[i] = x[i];
    
    obj_value = double_pb_->CostFunction(x_);
//     std::cout<<"obj_value = " << obj_value <<std::endl;
//     std::cout<<"eval_f fin"<<std::endl;
    return true; // 
}

bool BasisFunctionNLP::eval_grad_f (Index n, const Number * x, bool new_x, Number * grad_f)
{
//     std::cout<<"eval_grad_f"<<std::endl;
    for (int i=0;i<n;i++)
    {
        Fx_[i].value() = x[i];
        Fx_[i].derivatives() = Eigen::VectorXd::Unit(n, i);
    }
    
    AD crit = Fdouble_pb_->CostFunction(Fx_);
//     std::cout<<"obj_value = " << crit <<std::endl;
    for (int i=0;i<n;i++)
        grad_f[i] = crit.derivatives()[i];
//     std::cout<<"eval_grad_f fin"<<std::endl;
    return true;
}

bool BasisFunctionNLP::eval_g (Index n, const Number * x, bool new_x, Index m, Number * g)
{
//     std::cout<<"eval_g"<<std::endl;
    for (int i=0;i<n;i++)
        x_[i] = x[i];
 
    std::vector<double> ctr = double_pb_->Constraints(x_);
    for (int i=0;i<m;i++)
        g[i] = ctr[i];
//     std::cout<<"eval_g fin"<<std::endl;
    return true;
}

bool BasisFunctionNLP::eval_jac_g (Index n, const Number * x, bool new_x,
		   Index m, Index nele_jac, Index * iRow, Index * jCol,
		   Number * values)
{
//     std::cout<<"eval_jac_g"<<std::endl;
	if (values == NULL)
    {
        uint cpt =0;
        for (int i=0;i<m;i++)   for (int j=0;j<n;j++)
        {
            iRow[cpt] = i;
            jCol[cpt] = j;
            cpt++;
        }
    }
	else
    {
        for (int i=0;i<n;i++)
        {
            Fx_[i].value() = x[i];
            Fx_[i].derivatives() = Eigen::VectorXd::Unit(n, i);
        }

        std::vector<AD> ctr = Fdouble_pb_->Constraints(Fx_);
        uint cpt=0;
        for (int i=0;i<m;i++) 
        {
            for (int j=0;j<n;j++)
                values[cpt++] = ctr[i].derivatives()[j];
        }
	  }
//     std::cout<<"eval_jac_g fin"<<std::endl;
	return true;
}

bool BasisFunctionNLP::eval_h (Index n, const Number * x, bool new_x,
	       Number obj_factor, Index m, const Number * lambda,
	       bool new_lambda, Index nele_hess, Index * iRow,
	       Index * jCol, Number * values)
{
	if (values == NULL)
	  {

          
    }
	else
	  {
		  // return the values
	  }

	return true;
}

void BasisFunctionNLP::finalize_solution (SolverReturn status,
			  Index n, const Number * x, const Number * z_L,
			  const Number * z_U, Index m, const Number * g,
			  const Number * lambda, Number obj_value,
			  const IpoptData * ip_data,
			  IpoptCalculatedQuantities * ip_cq)
{
	// here is where we would store the solution to variables, or write to a file, etc
	// so we could use the solution. Since the solution is displayed to the console,
	// we currently do nothing here.
    for (int i=0;i<n;i++)
    {
        x_[i] = x[i];
    }
    double obj1 = double_pb_->CostFunction(x_);
    final_mat_ =  double_pb_->GetMatrix();
//     std::cout<<"resultat = \n "<<mat <<std::endl;
//     std::cout<<"obj_value = "<< obj1 <<" : "<< mat.determinant()<<std::endl;    
    
    std::vector< std::vector<double> > root = double_pb_->getRoot();
        
    std::string title,filename;
    std::string name=criteria_type_;
    
    filename = name+std::to_string(order_)+".txt";

    std::ofstream file(filename);
    
    file<<"order = "<< order_ <<std::endl<<std::endl;
    file<<" // roots"<<std::endl;
    for (int i=0;i<root.size();i++)
    {
        for (int j=0;j<root[i].size();j++)
            file<<"\troot["<<i<<"]["<<j<<"] = "<< root[i][j]<<std::endl;
    }
    
    file<<std::endl;
    file<<" // matrix "<<std::endl;
    for (int i=0;i<order_+1;i++)
    {
        for (int j=0;j<order_+1;j++)
            file<<"\tM("<<i<<","<<j<<") = " << std::setprecision(24) << final_mat_(i,j)<<";"<<std::endl;
    }
    file<<std::endl<<std::endl;
    
    file<<"crit : "<< obj1 <<std::endl;

    
    file.close();
//     double_pb_->ApproximateRoots();
}

void BasisFunctionNLP::get_matrix( Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> & m)
{
    m = final_mat_;
}

#endif // IPOPT_FOUND
