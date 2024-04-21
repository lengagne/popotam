#ifndef ChooseBasisFunction_H
#define ChooseBasisFunction_H
#include <vector>
#include "AbstractBasisFunction.h"

class ChooseBasisFunction
{
public:
    ChooseBasisFunction();
    
    void choose(AbstractBasisFunction** bf, 
                const std::string& basis_type);

    std::string get_basis_type(uint index)
    {
        return basis_functions_[index];
    }

    uint get_nb_basis_type()
    {
        return basis_functions_.size();
    }

private:
    std::map<uint,std::string> basis_functions_;
};

#endif // ChooseBasisFunction_H
