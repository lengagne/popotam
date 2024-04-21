#ifndef DiscretizationSolver_H
#define DiscretizationSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"


class DiscretizationSolver: public AbstractSolver
{
    public:
        DiscretizationSolver(AbstractCSP* pb);

        virtual ~DiscretizationSolver();

        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out);

        void set_discretisation_step(unsigned int n)
        {
            nb_discret_ = n;
        }

        virtual param_optim solve_optim(double eps=1e-3);


    protected:
    private:
        unsigned int nb_discret_;

};

#endif // DiscretizationSolver_H
