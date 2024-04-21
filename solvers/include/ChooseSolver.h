#ifndef ChooseSolver_H
#define ChooseSolver_H

#include "AbstractSolver.h"
#include "SolverOptions.h"

void PopotamSetSolver( AbstractCSP* pb,
                AbstractSolver** solver, 
                const SolverOptions& options);


void PopotamPrintAllOptions();

#endif // ChooseSolver_H
