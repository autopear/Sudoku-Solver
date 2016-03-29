#include "gridboard.h"
#include "sudokuboard.h"
#include "abstractsolver_p.h"
#include "abstractsolver.h"

using namespace CIS5603;

AbstractSolver::AbstractSolver(GridBoard *table, SudokuBoard *board, QObject *parent) :
    QThread(parent),
    m_private(new AbstractSolverPrivate())
{
    m_private->table = table;
    m_private->board = board;
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
    if (m_private->table && m_private->board)
    {
        int rows = m_private->table->rows();
        int columns = m_private->table->columns();

        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<columns; j++)
            {
                if (m_private->table->value(i, j) == 0)
                    return true;
            }
        }
    }

    return false;
}

bool AbstractSolver::computeNextValue(int *value, int *row, int *column)
{
    if (value)
        *value = 0;
    if (row)
        *row = -1;
    if (column)
        *column = -1;

    return false;
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
        if (canCompute())
        {
            int v, r, c;
            if (computeNextValue(&v, &r, &c))
                emit computed(v, r, c);
            else
                return;
        }
        else
            emit terminated(tr("The Sudoku puzzle is already solved."));
    }
    else
    {
        m_private->stopped = false;

        if (canCompute())
        {
            while (!m_private->stopped && canCompute())
            {
                int v, r, c;
                if (computeNextValue(&v, &r, &c))
                    emit computed(v, r, c);
                else
                    return;
            }
        }
        else
            emit terminated(tr("The Sudoku puzzle is already solved."));
    }
}

AbstractSolverPrivate::AbstractSolverPrivate()
{
    table = 0;
    board = 0;
    step = true;
    stopped = false;
}

AbstractSolverPrivate::~AbstractSolverPrivate()
{
}
