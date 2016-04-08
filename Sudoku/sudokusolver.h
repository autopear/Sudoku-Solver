#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QList>
#include <QObject>
#include <QPair>

namespace CIS5603
{

class SudokuBoard;
class SudokuSolverPrivate;

class SudokuSolver : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Idle,
        Running,
        Finished,
        Terminated
    };

    explicit SudokuSolver(SudokuBoard *board, QObject *parent = 0);
    ~SudokuSolver();

    State state() const;

    QString algorithm() const;

    QList<QPair<QPoint, int> > computedValues() const;

    void setValues(int **values);

signals:
    void finished();
    void terminated(const QString &message);

public slots:
    void reset();
    void setAlgorithm(const QString &algorithm);
    void compute();
    void stop();

private slots:
    void onTerminated(const QString &message);
    void onFinished();

private:
    SudokuSolverPrivate *m_private;
};

}

#endif // SUDOKUSOLVER_H
