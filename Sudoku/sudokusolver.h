#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QObject>

namespace CIS5603
{

class GridBoard;
class SudokuBoard;
class SudokuSolverPrivate;

class SudokuSolver : public QObject
{
    Q_OBJECT
public:
    explicit SudokuSolver(GridBoard *table, SudokuBoard *board, QObject *parent = 0);
    ~SudokuSolver();

    QString algorithm() const;

    bool isRunning() const;

    qint64 lastStepTime() const;
    qint64 totalTime() const;

signals:
    void proceeded(int value, int row, int column, qint64 stepTime, qint64 totalTime);
    void finished(qint64 totalTime);
    void terminated(qint64 totalTime, const QString &message);

public slots:
    void reset();
    void setAlgorithm(const QString &algorithm);
    void nextStep();
    void goToEnd();
    void stop();

private slots:
    void onComputed(int value, int row, int column);
    void onTerminated(const QString &message);
    void onFinished();

private:
    SudokuSolverPrivate *m_private;
};

}

#endif // SUDOKUSOLVER_H
