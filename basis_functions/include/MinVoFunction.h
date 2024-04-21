#ifdef IPOPT_FOUND

#ifndef MinVoFunction_H
#define MinVoFunction_H

#include "OptimizedBasisFunction.h"


class MinVoFunction: public OptimizedBasisFunction
{
    public:
       
        MinVoFunction();
        
        ~MinVoFunction()
        {
            
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H

#endif //  IPOPT_FOUND
