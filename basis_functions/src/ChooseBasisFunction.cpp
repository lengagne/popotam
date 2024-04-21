#include "ChooseBasisFunction.h"

#include "ApproxMinvoFunction.h"
#include "MinVoFunction.h"
#include "MinNoFunction.h"
#include "MinVariance.h"
#include "BernsteinFunction.h"
#include "BSplinesFunction.h"
#include "RecursiveBSplinesFunction.h"
#include "RecursiveBSplinesFunction2.h"


ChooseBasisFunction::ChooseBasisFunction()
{
    uint cpt = 0;
    basis_functions_[cpt++] = "Bernstein";      // 0 
#ifdef IPOPT_FOUND    
    basis_functions_[cpt++] = "MinVariance";    // 1
#endif    
    basis_functions_[cpt++] = "BSplines";       // 2
#ifdef IPOPT_FOUND
    basis_functions_[cpt++] = "MinNo";          // 3 
#endif        
#ifdef IPOPT_FOUND
    basis_functions_[cpt++] = "MinVo";          // 4 
#endif    
    basis_functions_[cpt++] = "ApproxMinVo";    // 5    

    basis_functions_[cpt++] = "RecursiveBSplines";// 6 
    basis_functions_[cpt++] = "RecursiveBSplines2";// 7 
}

void ChooseBasisFunction::choose(AbstractBasisFunction** bf, 
                                 const std::string& basis_type)
{
    uint cpt = 0;
    
    if (basis_type == "Bernstein")   
    {
        *bf = new BernsteinFunction(); 
        return;
    }

#ifdef IPOPT_FOUND 
    if (basis_type == "MinVariance") 
    {
        *bf = new MinVariance(); 
        return;
    }
#endif // IPOPT_FOUND
    
    
    if (basis_type == "BSplines")    
    {
        *bf = new BSplinesFunction(); 
        return;
    }

#ifdef IPOPT_FOUND     
    if (basis_type == "MinNo")
    {
        *bf = new MinNoFunction(); 
        return;
    }
#endif // IPOPT_FOUND
    
#ifdef IPOPT_FOUND    
    if (basis_type == "MinVo")
    {
        *bf = new MinVoFunction();
        return;
    }    
#endif // IPOPT_FOUND


    if (basis_type == "ApproxMinVo")
    {
        *bf = new ApproxMinvoFunction(); 
            return;
    }
   
    if (basis_type == "RecursiveBSplines")    
    {
        *bf = new RecursiveBSplinesFunction(); 
        return;
    }


    if (basis_type == "RecursiveBSplines2")
    {
        *bf = new RecursiveBSplinesFunction2(); 
        return;
    }
    
    std::cerr<<"Error in FILE "<< __FILE__<<" at line "<< __LINE__<<" the basis_type "<<basis_type <<" is not defined yet"<<std::endl;
    exit(123);
}


