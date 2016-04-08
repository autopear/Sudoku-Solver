#include "abstractsolver_p.h"
#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard_p.h"
#include "sudokuboard.h"
#include "heuristicsearchsolver.h"

using namespace CIS5603;

HeuristicSearchSolver::HeuristicSearchSolver(SudokuBoard *board, QObject *parent) :
    AbstractSolver(board, parent),
    m_private(new HeuristicSearchSolverPrivate())
{
}

HeuristicSearchSolver::~HeuristicSearchSolver()
{
}

QString HeuristicSearchSolver::algorithm() const
{
    return tr("Heuristic Search");
}

QList<QPair<QPoint, int> > HeuristicSearchSolver::computedValues() const
{
    return m_private->values;
}

bool HeuristicSearchSolver::compute()
{
    bool foundSolution = false;

    computeSudoku(values(), &(m_private->values), &foundSolution);

    return foundSolution;
}

void HeuristicSearchSolver::computeSudoku(int **gridValues, QList<QPair<QPoint, int> > *computedValues, bool *foundSolution)
{
    if (*foundSolution || stopped() || !MainWindow::sharedInstance())
        return;

    int v;
    QMap<QPoint, QList<int> > vs;

    QPoint cell = GridBoard::getBestCell(gridValues, board(), &v, &vs, MainWindow::sharedInstance()->multithreadingEnabled());

    if (cell == QPoint(-1, -1))
    {
        int count = qMax(board()->rows(), board()->rows()) + 1;
        QList<int> availables;
        foreach (QPoint key, vs.keys())
        {
            if (*foundSolution || stopped())
                break;

            QList<int> tmp = vs.value(key);
            if (tmp.size() < count)
            {
                cell = key;
                availables = tmp;
            }
        }

        foreach (int value, availables)
        {
            if (*foundSolution || stopped())
                break;

            int **copyValues = (int **)malloc(sizeof(int *) * board()->rows());
            for (int i=0; i<board()->rows() ; i++)
            {
                if (*foundSolution || stopped())
                {
                    delete copyValues;
                    return;
                }

                int *r = (int *)malloc(sizeof(int) * board()->columns());
                for (int j=0; j<board()->columns(); j++)
                    r[j] = gridValues[i][j];
                copyValues[i] = r;
            }
            copyValues[cell.x()][cell.y()] = value;

            if (canCompute(copyValues, board()->rows(), board()->columns()))
                computeSudoku(copyValues, computedValues, foundSolution);
            else
            {
                *foundSolution = true;

                for (int i=0; i<board()->rows(); i++)
                {
                    for (int j=0; j<board()->columns(); j++)
                    {
                        if (stopped())
                        {
                            delete copyValues;
                            return;
                        }

                        int newValue = copyValues[i][j];
                        if (values()[i][j] != newValue)
                            computedValues->append(QPair<QPoint, int>(QPoint(i, j), newValue));
                    }
                }
            }

            delete copyValues;
        }
    }
    else
    {
        int **copyValues = (int **)malloc(sizeof(int *) * board()->rows());
        for (int i=0; i<board()->rows() ; i++)
        {
            if (*foundSolution || stopped())
            {
                delete copyValues;
                return;
            }

            int *r = (int *)malloc(sizeof(int) * board()->columns());
            for (int j=0; j<board()->columns(); j++)
                r[j] = gridValues[i][j];
            copyValues[i] = r;
        }
        copyValues[cell.x()][cell.y()] = v;

        if (canCompute(copyValues, board()->rows(), board()->columns()))
            computeSudoku(copyValues, computedValues, foundSolution);
        else
        {
            for (int i=0; i<board()->rows(); i++)
            {
                for (int j=0; j<board()->columns(); j++)
                {
                    if (stopped())
                    {
                        delete copyValues;
                        return;
                    }

                    int newValue = copyValues[i][j];
                    if (values()[i][j] != newValue)
                        computedValues->append(QPair<QPoint, int>(QPoint(i, j), newValue));
                }
            }

            *foundSolution = true;
        }
        delete copyValues;
    }
}

HeuristicSearchSolverPrivate::HeuristicSearchSolverPrivate()
{
    values = QList<QPair<QPoint, int> >();
}

HeuristicSearchSolverPrivate::~HeuristicSearchSolverPrivate()
{
    values.clear();
}
