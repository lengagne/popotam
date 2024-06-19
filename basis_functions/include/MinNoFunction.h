#ifdef IPOPT_FOUND

#ifndef MinNoFunction_H
#define MinNoFunction_H

#include "OptimizedBasisFunction.h"


class MinNoFunction: public OptimizedBasisFunction
{
    public:
       
        MinNoFunction();
        
        ~MinNoFunction()
        {
            
        }
        
        std::string get_name() const
        {
            return "MinNo";
        }        
        
    protected:
    private:
};

#endif // MinNoFunction_H

#endif //  IPOPT_FOUND
