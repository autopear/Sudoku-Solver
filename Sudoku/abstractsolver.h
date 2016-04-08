#ifndef ABSTRACTSOLVER_H
#define ABSTRACTSOLVER_H

//Sub-class this for every algorithm

#include <QThread>

namespace CIS5603
{

class AbstractSolverPrivate;
class SudokuBoard;

class AbstractSolver : public QThread
{
    Q_OBJECT
public:
    explicit AbstractSolver(SudokuBoard *board, QObject *parent = 0);
    ~AbstractSolver();

    //Override this for the algorithm name
    virtual QString algorithm() const;

    SudokuBoard *board();

    int **values();
    void setValues(int **values);

    virtual QList<QPair<QPoint, int> > computedValues() const;

    //Override this for the actual function to compute the next available value
    virtual bool compute();

    static bool canCompute(int **values, int rows, int columns);

    bool stopped() const;

signals:
    void interrupted(const QString &message);

public slots:
    void stop();

protected:
    void run();

private:
    AbstractSolverPrivate *m_private;
};

}

#endif // ABSTRACTSOLVER_H
