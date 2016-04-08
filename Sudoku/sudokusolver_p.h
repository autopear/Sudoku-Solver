#ifndef SUDOKUSOLVER_P_H
#define SUDOKUSOLVER_P_H

#include "sudokusolver.h"

namespace CIS5603
{

class AbstractSolver;
class SudokuBoard;

class SudokuSolverPrivate
{
    friend class SudokuSolver;
protected:
    SudokuSolverPrivate();
    ~SudokuSolverPrivate();

    SudokuBoard *board;

    AbstractSolver *solver;

    SudokuSolver::State state;

    QList<QPair<QPoint, int> > values;
};

}

#endif // SUDOKUSOLVER_P_H
