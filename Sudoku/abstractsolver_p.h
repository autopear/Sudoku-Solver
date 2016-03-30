#ifndef ABSTRACTSOLVER_P_H
#define ABSTRACTSOLVER_P_H

namespace CIS5603
{

class AbstractSolverPrivate
{
    friend class AbstractSolver;
protected:
    AbstractSolverPrivate();
    ~AbstractSolverPrivate();

    bool step;
    bool stopped;
};

}

#endif // ABSTRACTSOLVER_P_H
