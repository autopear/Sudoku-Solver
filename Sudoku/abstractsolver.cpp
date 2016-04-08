#include "sudokuboard.h"
#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "abstractsolver_p.h"
#include "abstractsolver.h"

using namespace CIS5603;

AbstractSolver::AbstractSolver(SudokuBoard *board, QObject *parent) :
    QThread(parent),
    m_private(new AbstractSolverPrivate())
{
    m_private->board = board;

    if (board->rows() > 0 && board->columns() > 0)
    {
        m_private->values = (int **)malloc(sizeof(int *) * board->rows() );
        for (int i=0; i<board->rows() ; i++)
        {
            int *r = (int *)malloc(sizeof(int) * board->columns());
            for (int j=0; j<board->columns(); j++)
                r[j] = 0;
            m_private->values[i] = r;
        }
    }
    else
        m_private->values = 0;
}

AbstractSolver::~AbstractSolver()
{
    delete m_private;
}

QString AbstractSolver::algorithm() const
{
    return QString();
}

SudokuBoard *AbstractSolver::board()
{
    return m_private->board;
}

int **AbstractSolver::values()
{
    return m_private->values;
}

void AbstractSolver::setValues(int **values)
{
    for (int i=0; i<m_private->board->rows(); i++)
    {
        for (int j=0; j<m_private->board->columns(); j++)
            m_private->values[i][j] = values[i][j];
    }
}

QList<QPair<QPoint, int> > AbstractSolver::computedValues() const
{
    return QList<QPair<QPoint, int> >();
}

bool AbstractSolver::compute()
{
    return false;
}

void AbstractSolver::stop()
{
    m_private->stopped = true;
}

bool AbstractSolver::stopped() const
{
    return m_private->stopped;
}

void AbstractSolver::run()
{
    m_private->stopped = false;

    if (!canCompute(m_private->values, m_private->board->rows(), m_private->board->columns()))
        emit interrupted(tr("The Sudoku puzzle is solved."));
    else if (compute())
    {
    }
    else
        emit interrupted(tr("The game cannot be solved."));

    m_private->stopped = true;
}

bool AbstractSolver::canCompute(int **values, int rows, int columns)
{
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<columns; j++)
        {
            if (values[i][j] == 0)
                return true;
        }
    }
    return false;
}

AbstractSolverPrivate::AbstractSolverPrivate()
{
    stopped = false;
}

AbstractSolverPrivate::~AbstractSolverPrivate()
{
    if (values)
        delete values;
}
