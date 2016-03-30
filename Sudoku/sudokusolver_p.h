#ifndef SUDOKUSOLVER_P_H
#define SUDOKUSOLVER_P_H

#include "sudokusolver.h"

class QElapsedTimer;

namespace CIS5603
{

class AbstractSolver;

class SudokuSolverPrivate
{
    friend class SudokuSolver;
protected:
    SudokuSolverPrivate();
    ~SudokuSolverPrivate();

    AbstractSolver *solver;

    QElapsedTimer *timer;

    qint64 totalTime;
    qint64 lastTime;
};

}

#endif // SUDOKUSOLVER_P_H
