#ifndef BissectionIntervalSolver_H
#define BissectionIntervalSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"


class BissectionIntervalSolver: public AbstractSolver
{
    public:
        BissectionIntervalSolver(   AbstractCSP* pb,
                                    const std::string& bissection_type="MinFirst");
                                    

        virtual ~BissectionIntervalSolver();
        

        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out);

        virtual param_optim solve_optim(double eps = 1e-3);

    protected:
    private:

};

#endif // BissectionIntervalSolver_H
