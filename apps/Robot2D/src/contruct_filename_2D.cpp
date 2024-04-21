#include "construct_filename_2D.h"

std::string construct_filename_2D(
        uint ndof,
        uint npb,
        double precision,
        uint bissection_mode,
        uint type_bissection // intervale, basis, ...
        )
{
    std::string filename = "save2D";
    filename += "_ndof" + std::to_string(ndof);
    filename += "_npb" + std::to_string(npb);
    filename += "_prec" + std::to_string(precision);
    filename += "_bimo" + std::to_string(bissection_mode);
    filename += "_tybi" + std::to_string(type_bissection);
    filename += "_v1.sop"; // Save Optim Process
    
    return filename;
}

