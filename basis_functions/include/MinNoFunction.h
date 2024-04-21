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
        
    protected:
    private:
};

#endif // MinNoFunction_H

#endif //  IPOPT_FOUND
