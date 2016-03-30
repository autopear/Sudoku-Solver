#include "gridboard.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "heuristicsearchsolver.h"

using namespace CIS5603;

HeuristicSearchSolver::HeuristicSearchSolver(QObject *parent) :
    AbstractSolver(parent)
{
}

HeuristicSearchSolver::~HeuristicSearchSolver()
{
}

QString HeuristicSearchSolver::algorithm() const
{
    return tr("Heuristic Search");
}

int HeuristicSearchSolver::computeNextValue(int *value, int *row, int *column)
{
    int bestValue;
    QMap<QPoint, QList<int> > bestValues;
    QPoint cell = MainWindow::sharedInstance()->gridBoard()->getBestCell(&bestValue, &bestValues, MainWindow::sharedInstance()->multithreadingEnabled());

    if (cell != QPoint(-1, -1))
    {
        *value = bestValue;
        *row = cell.x();
        *column = cell.y();
        return 1;
    }

    if (bestValues.isEmpty())
        return 0;

    if (value)
        *value = 0;
    if (row)
        *row = -1;
    if (column)
        *column = -1;

    emit interrupted(tr("The game cannot be solved."));

    return -1;
}
