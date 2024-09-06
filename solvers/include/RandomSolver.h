#ifndef RandomSolver_H
#define RandomSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"


class RandomSolver: public AbstractSolver
{
    public:
        RandomSolver(AbstractCSP* pb,
                     const PopotamOptions *options);

        virtual ~RandomSolver();

        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out);

        virtual param_optim solve_optim(double eps=1e-3);


    protected:
    private:
        int nb_random_;

};

#endif // RandomSolver_H
