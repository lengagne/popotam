#ifndef BissectionBasisFunctionSolver_H
#define BissectionBasisFunctionSolver_H
#include <iostream>
#include <fstream>

#include "BasisFunctionSolver.h"
#include "IntervalEstimator.h"


class BissectionBasisFunctionSolver: public BasisFunctionSolver
{
    public:
        BissectionBasisFunctionSolver(AbstractCSP* pb, 
                                      const PopotamOptions *options);

        virtual ~BissectionBasisFunctionSolver();
        
        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out);      
        
    protected:
    private:

        virtual void init(double eps);

};

#endif // BissectionBasisFunctionSolver_H
