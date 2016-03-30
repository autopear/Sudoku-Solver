#include <QElapsedTimer>
#include "heuristicsearchsolver.h"
#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "sudokusolver_p.h"
#include "sudokusolver.h"

using namespace CIS5603;

SudokuSolver::SudokuSolver(QObject *parent) :
    QObject(parent),
    m_private(new SudokuSolverPrivate())
{
    m_private->timer = new QElapsedTimer();
}

SudokuSolver::~SudokuSolver()
{
    delete m_private;
}

QString SudokuSolver::algorithm() const
{
    if (m_private->solver)
        return m_private->solver->algorithm();
    else
        return QString();
}

bool SudokuSolver::isRunning() const
{
    if (m_private->solver)
        return m_private->solver->isRunning();
    else
        return false;
}

qint64 SudokuSolver::lastStepTime() const
{
    return m_private->lastTime;
}

qint64 SudokuSolver::totalTime() const
{
    return m_private->totalTime;
}

void SudokuSolver::reset()
{
    if (!m_private->solver)
        return;

    if (m_private->solver->isRunning())
        return;

    if (m_private->timer->isValid())
        m_private->timer->invalidate();

    m_private->lastTime = 0;
    m_private->totalTime = 0;
}

void SudokuSolver::setAlgorithm(const QString &algorithm)
{
    if (isRunning())
        return;

    if (m_private->solver)
    {
        if (algorithm.isEmpty())
        {
            delete m_private->solver;
            m_private->solver = 0;
        }
        else
        {
            if (algorithm.compare(m_private->solver->algorithm()) != 0)
            {
                delete m_private->solver;

                if (algorithm.compare(tr("Heuristic Search")) == 0)
                {
                    m_private->solver = new HeuristicSearchSolver(0);
                    connect(m_private->solver, SIGNAL(computed(int,int,int)),
                            this, SLOT(onComputed(int,int,int)));
                    connect(m_private->solver, SIGNAL(interrupted(QString)),
                            this, SLOT(onTerminated(QString)));
                    connect(m_private->solver, SIGNAL(finished()),
                            this, SLOT(onFinished()));
                }
                else
                    m_private->solver = 0;
            }
        }
    }
    else
    {
        if (!algorithm.isEmpty())
        {
            if (algorithm.compare(tr("Heuristic Search")) == 0)
            {
                m_private->solver = new HeuristicSearchSolver(0);
                connect(m_private->solver, SIGNAL(computed(int,int,int)),
                        this, SLOT(onComputed(int,int,int)));
                connect(m_private->solver, SIGNAL(interrupted(QString)),
                        this, SLOT(onTerminated(QString)));
                connect(m_private->solver, SIGNAL(finished()),
                        this, SLOT(onFinished()));
            }
            else
                m_private->solver = 0;
        }
    }
}

void SudokuSolver::nextStep()
{
    if (!m_private->solver)
        return;

    if (m_private->solver->isRunning())
        return;

    m_private->timer->restart();
    m_private->solver->setStepByStep(true);
    m_private->solver->start();
}

void SudokuSolver::goToEnd()
{
    if (!m_private->solver)
        return;

    if (m_private->solver->isRunning())
        return;

    m_private->timer->restart();
    m_private->solver->setStepByStep(false);
    m_private->solver->start();
}

void SudokuSolver::stop()
{
    if (!m_private->solver)
        return;

    if (!m_private->solver->isRunning())
        return;

    m_private->solver->stop();
}

void SudokuSolver::onComputed(int value, int row, int column)
{
    m_private->lastTime = m_private->timer->elapsed();
    m_private->totalTime += m_private->lastTime;

    emit proceeded(value, row, column, m_private->lastTime, m_private->totalTime);

    m_private->timer->restart();
}

void SudokuSolver::onTerminated(const QString &message)
{
    m_private->totalTime += m_private->timer->elapsed();

    m_private->timer->invalidate();

    emit terminated(m_private->totalTime, message);
}

void SudokuSolver::onFinished()
{
    if (m_private->timer->isValid())
    {
        m_private->lastTime = m_private->timer->elapsed();
        m_private->totalTime += m_private->lastTime;

        m_private->timer->invalidate();

        emit finished(m_private->totalTime);
    }
}

SudokuSolverPrivate::SudokuSolverPrivate()
{
    solver = 0;
    timer = 0;

    totalTime = 0;
    lastTime = 0;
}

SudokuSolverPrivate::~SudokuSolverPrivate()
{
    if (solver)
    {
        solver->stop();
        solver->deleteLater();
    }

    if (timer->isValid())
        timer->invalidate();
    delete timer;
}
