#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "abstractsolver_p.h"
#include "abstractsolver.h"

using namespace CIS5603;

AbstractSolver::AbstractSolver(QObject *parent) :
    QThread(parent),
    m_private(new AbstractSolverPrivate())
{
}

AbstractSolver::~AbstractSolver()
{
    delete m_private;
}

QString AbstractSolver::algorithm() const
{
    return QString();
}

void AbstractSolver::setStepByStep(bool enabled)
{
    m_private->step = enabled;
}

bool AbstractSolver::canCompute() const
{
    GridBoard *table = MainWindow::sharedInstance()->gridBoard();

    if (table)
    {
        int rows = table->rows();
        int columns = table->columns();

        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<columns; j++)
            {
                if (table->value(i, j) == 0)
                    return true;
            }
        }
    }

    return false;
}

int AbstractSolver::computeNextValue(int *value, int *row, int *column)
{
    if (value)
        *value = 0;
    if (row)
        *row = -1;
    if (column)
        *column = -1;

    emit interrupted(tr("The algorithm is not implemented."));

    return -1;
}

void AbstractSolver::stop()
{
    m_private->stopped = true;
}

void AbstractSolver::run()
{
    m_private->stopped = false;

    if (m_private->step)
    {
        int v, r, c;
        int res = computeNextValue(&v, &r, &c);
        if (res == 1)
            emit computed(v, r, c);
        else if (res == 0)
                emit interrupted(tr("The Sudoku puzzle is solved."));
        else
            return;
    }
    else
    {
        m_private->stopped = false;

        while (!m_private->stopped)
        {
            int v, r, c;
            int res = computeNextValue(&v, &r, &c);
            if (res == 1)
                emit computed(v, r, c);
            else if (res == 0)
                break;
            else
                return;
        }

        emit interrupted(tr("The Sudoku puzzle is solved."));
    }
}

AbstractSolverPrivate::AbstractSolverPrivate()
{
    step = true;
    stopped = false;
}

AbstractSolverPrivate::~AbstractSolverPrivate()
{
}
