#ifndef HEURISTICSEARCHSOLVER_H
#define HEURISTICSEARCHSOLVER_H

#include "abstractsolver.h"

namespace CIS5603
{

class HeuristicSearchSolver : public AbstractSolver
{
    Q_OBJECT
public:
    explicit HeuristicSearchSolver(QObject *parent = 0);
    ~HeuristicSearchSolver();

    virtual QString algorithm() const;

    virtual int computeNextValue(int *value, int *row, int *column);
};

}

#endif // HEURISTICSEARCHSOLVER_H
