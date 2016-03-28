#ifndef ABSTRACTSOLVER_H
#define ABSTRACTSOLVER_H

//Sub-class this for every algorithm

#include <QThread>

namespace CIS5603
{

class GridBoard;
class SudokuBoard;
class AbstractSolverPrivate;

class AbstractSolver : public QThread
{
    Q_OBJECT
public:
    explicit AbstractSolver(GridBoard *table, SudokuBoard *board, QObject *parent = 0);
    ~AbstractSolver();

    //Override this for the algorithm name
    virtual QString algorithm() const;

    void setStepByStep(bool enabled);

    bool canCompute() const;

    //Override this for the actual function to compute the next available value
    virtual bool computeNextValue(int *value, int *row, int *column);

signals:
    void computed(int value, int row, int column);
    void terminated(const QString &message);

public slots:
    void stop();

protected:
    void run();

private:
    AbstractSolverPrivate *m_private;
};

}

#endif // ABSTRACTSOLVER_H
