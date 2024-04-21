#ifndef __CONSTRUCT_FILENAME_2D__
#define __CONSTRUCT_FILENAME_2D__

#include <string>

std::string construct_filename_2D(
        uint ndof,
        uint npb,
        double precision,
        uint bissection_mode,
        uint type_bissection // intervale, basis, ...
        );

    
#endif
