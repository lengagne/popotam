#ifndef __MINVOPROBLEM__
#define __MINVOPROBLEM__

#include <Eigen/Dense>
#include <iostream>
#include "Polynomial.h"

template <typename TYPE>
class BasisFunctionProblem
{
public : 
    
        BasisFunctionProblem(   uint size,
                                const std::string & criteria_type)
        {
            criteria_type_ = criteria_type;
            one_ = 1.0;
            n_ = size;
            odd_ = n_%2;
            lambda_.resize(n_+1);
            A_.resize(n_+1,n_+1);            
            t = Polynomial<TYPE>();
        }
        
        std::vector<TYPE> Constraints( std::vector<TYPE> & x);

        TYPE CostFunction(std::vector<TYPE>&  x);
        
        void GetConstraintLimits( std::vector<double>& low, 
                                  std::vector<double>& high);
       
        void GetParamLimits( std::vector<double>& low, 
                                  std::vector<double>& high)
        {
            low.clear();
            high.clear();            
            for (int i=0;i<nb_root_;i++)
            {
                low.push_back(-1.0);
                high.push_back(1.0);                
            }
//             low = Xmin_;
//             high = Xmax_;
        }
        
        Eigen::Matrix<TYPE,Eigen::Dynamic,Eigen::Dynamic> GetMatrix()
        {
            return A_;
        }

        uint GetNext(uint i)const;
        
        uint GetNumberConstraints();        
        uint GetNumberParam();
        
        std::vector< std::vector<TYPE> > getRoot() const
        {
            return root_;
        }
        

private:    
        uint n_ =0 , nb_root_ = 0, nb_param_=0;
        
        uint nb_add_ctr_ = 0;
        
        bool odd_;
        
        Eigen::Matrix<TYPE,Eigen::Dynamic,Eigen::Dynamic> A_;
        
        std::vector<Polynomial<TYPE> > lambda_;
        
        Polynomial<TYPE> one_;
        
        Eigen::Matrix<TYPE,Eigen::Dynamic,1> bi_,e_;
        std::vector< std::vector<TYPE> > root_;
        
        std::vector<TYPE> constraints_;
        
        void set_x(const std::vector<TYPE>& x);
        
        void set_matrix(const std::vector<TYPE>& x);
        
        void set_matrix( );
        
        Polynomial<TYPE> t;
        
        bool add_cost_ctr_ = false;
        
        bool add_root_order_ctr_ = true;
        
        std::vector<double> Xmin_,Xmax_;
        
        
        std::string criteria_type_ = "Minvo";
        

};

#include "BasisFunctionProblem.hxx"

#endif // __MINVOPROBLEM__
