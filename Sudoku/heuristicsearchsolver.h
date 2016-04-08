#ifndef HEURISTICSEARCHSOLVER_H
#define HEURISTICSEARCHSOLVER_H

#include <QList>
#include <QPair>
#include <QPoint>
#include "abstractsolver.h"

namespace CIS5603
{

class HeuristicSearchSolverPrivate;
class SudokuBoard;

class HeuristicSearchSolver : public AbstractSolver
{
    Q_OBJECT
public:
    explicit HeuristicSearchSolver(SudokuBoard *board, QObject *parent = 0);
    ~HeuristicSearchSolver();

    virtual QString algorithm() const;

    virtual QList<QPair<QPoint, int> > computedValues() const;

    virtual bool compute();

    //Recursive function
    void computeSudoku(int **gridValues,
                       QList<QPair<QPoint, int> > *computedValues,
                       bool *foundSolution);

private:
    HeuristicSearchSolverPrivate *m_private;
};

}

#endif // HEURISTICSEARCHSOLVER_H
