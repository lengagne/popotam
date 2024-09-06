#ifndef ChooseSolver_H
#define ChooseSolver_H

#include "AbstractSolver.h"
#include "PopotamOptions.h"

void PopotamSetSolver( AbstractCSP* pb,
                AbstractSolver** solver, 
                const PopotamOptions* options);


void PopotamPrintAllOptions();

#endif // ChooseSolver_H
