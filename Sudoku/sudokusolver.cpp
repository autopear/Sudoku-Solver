#include "heuristicsearchsolver.h"
#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "sudokusolver_p.h"
#include "sudokusolver.h"

using namespace CIS5603;

SudokuSolver::SudokuSolver(SudokuBoard *board, QObject *parent) :
    QObject(parent),
    m_private(new SudokuSolverPrivate())
{
    m_private->board = board;
}

SudokuSolver::~SudokuSolver()
{
    delete m_private;
}

SudokuSolver::State SudokuSolver::state() const
{
    return m_private->state;
}

QString SudokuSolver::algorithm() const
{
    if (m_private->solver)
        return m_private->solver->algorithm();
    else
        return QString();
}

QList<QPair<QPoint, int> > SudokuSolver::computedValues() const
{
    return m_private->values;
}

void SudokuSolver::setValues(int **values)
{
    if (m_private->solver && m_private->state != SudokuSolver::Running)
        m_private->solver->setValues(values);
}

void SudokuSolver::reset()
{
    if (!m_private->solver)
        return;

    if (m_private->solver->isRunning())
        return;

    m_private->values.clear();

    m_private->state = SudokuSolver::Idle;
}

void SudokuSolver::setAlgorithm(const QString &algorithm)
{
    if (m_private->state == SudokuSolver::Running)
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
                    m_private->solver = new HeuristicSearchSolver(m_private->board, 0);
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
                m_private->solver = new HeuristicSearchSolver(m_private->board, 0);
            else
                m_private->solver = 0;
        }
    }

    m_private->values.clear();
    m_private->state = SudokuSolver::Idle;
}

void SudokuSolver::compute()
{
    if (!m_private->solver)
        return;

    if (m_private->solver->isRunning())
        return;

    m_private->state = SudokuSolver::Running;

    m_private->values.clear();

    connect(m_private->solver, SIGNAL(interrupted(QString)),
            this, SLOT(onTerminated(QString)));
    connect(m_private->solver, SIGNAL(finished()),
            this, SLOT(onFinished()));

    m_private->solver->start();
}

void SudokuSolver::stop()
{
    if (!m_private->solver)
        return;

    if (!m_private->solver->isRunning())
        return;

    disconnect(m_private->solver, SIGNAL(interrupted(QString)),
               this, SLOT(onTerminated(QString)));
    disconnect(m_private->solver, SIGNAL(finished()),
               this, SLOT(onFinished()));

    m_private->solver->stop();

    m_private->state = SudokuSolver::Finished;
}

void SudokuSolver::onTerminated(const QString &message)
{
    m_private->state = SudokuSolver::Terminated;

    disconnect(m_private->solver, SIGNAL(interrupted(QString)),
               this, SLOT(onTerminated(QString)));
    disconnect(m_private->solver, SIGNAL(finished()),
               this, SLOT(onFinished()));

    emit terminated(message);
}

void SudokuSolver::onFinished()
{
    if (m_private->state == SudokuSolver::Running)
    {
        disconnect(m_private->solver, SIGNAL(interrupted(QString)),
                   this, SLOT(onTerminated(QString)));
        disconnect(m_private->solver, SIGNAL(finished()),
                   this, SLOT(onFinished()));

        m_private->values = m_private->solver->computedValues();

        m_private->state = SudokuSolver::Finished;

        emit finished();
    }
}

SudokuSolverPrivate::SudokuSolverPrivate()
{
    solver = 0;

    state = SudokuSolver::Idle;

    values = QList<QPair<QPoint, int> >();
}

SudokuSolverPrivate::~SudokuSolverPrivate()
{
    if (solver)
    {
        solver->blockSignals(true);
        solver->stop();
        solver->deleteLater();
    }
}
