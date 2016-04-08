#ifndef ABSTRACTSOLVER_P_H
#define ABSTRACTSOLVER_P_H

#include <QList>
#include <QPair>
#include <QPoint>

namespace CIS5603
{

class SudokuBoard;

class AbstractSolverPrivate
{
    friend class AbstractSolver;
protected:
    AbstractSolverPrivate();
    ~AbstractSolverPrivate();

    SudokuBoard *board;
    bool stopped;
    int **values;
};

class HeuristicSearchSolverPrivate
{
    friend class HeuristicSearchSolver;
protected:
    HeuristicSearchSolverPrivate();
    ~HeuristicSearchSolverPrivate();

    QList<QPair<QPoint, int> > values;
};

}

#endif // ABSTRACTSOLVER_P_H
