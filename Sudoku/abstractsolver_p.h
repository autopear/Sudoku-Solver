#ifndef ABSTRACTSOLVER_P_H
#define ABSTRACTSOLVER_P_H

namespace CIS5603
{

class GridBoard;
class SudokuBoard;

class AbstractSolverPrivate
{
    friend class AbstractSolver;
protected:
    AbstractSolverPrivate();
    ~AbstractSolverPrivate();

    GridBoard *table;
    SudokuBoard *board;

    bool step;
    bool stopped;
};

}

#endif // ABSTRACTSOLVER_P_H
