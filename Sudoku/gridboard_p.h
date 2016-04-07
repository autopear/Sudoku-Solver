#ifndef GRIDBOARD_P_H
#define GRIDBOARD_P_H

#include <QList>
#include <QPolygon>
#include <QThread>
#include "gridboard.h"

class QLabel;
class QSemaphore;

namespace CIS5603
{

class GridBoard;
class GridModel;

class GridBoardPrivate
{
    friend class GridBoard;
protected:
    GridBoardPrivate();
    ~GridBoardPrivate();

    void adjustFont();

    GridBoard *board;

    QLabel *label;
    QSize size;
    GridModel *model;

    int maxWidthNum;
};

class LineValuesChecker : public QThread
{
    Q_OBJECT
public:
    explicit LineValuesChecker(int line,
                               bool isRow,
                               int **gridValues,
                               int rows,
                               int columns,
                               QSemaphore *mutex,
                               QList<int> *usedValues,
                               QObject *parent = 0);
    ~LineValuesChecker() { }

public slots:
    void check();

protected:
    void run() { check(); }

private:
    int m_line;
    bool m_isRow;
    int **m_gridValues;
    int m_rows;
    int m_columns;
    QSemaphore *m_mutex;
    QList<int> *m_values;
};

class BlockValuesChecker : public QThread
{
    Q_OBJECT
public:
    explicit BlockValuesChecker(const QPolygon &block,
                                int **gridValues,
                                int rows,
                                int columns,
                                QSemaphore *mutex,
                                QList<int> *usedValues,
                                QObject *parent = 0);
    ~BlockValuesChecker() { }

public slots:
    void check();

protected:
    void run() { check(); }

private:
    QPolygon m_block;
    int **m_gridValues;
    int m_rows;
    int m_columns;
    QSemaphore *m_mutex;
    QList<int> *m_values;
};

class BestCellFinder : public QThread
{
    Q_OBJECT
public:
    explicit BestCellFinder(int **gridValues,
                            SudokuBoard *board,
                            int index,
                            QPoint *cell,
                            int *value,
                            QMap<QPoint, QList<int> > *values,
                            QSemaphore *mutex,
                            bool useThread,
                            QObject *parent = 0);
    ~BestCellFinder() { }

public slots:
    void search();

protected:
    void run() { search(); }

private:
    int **m_gridValues;
    SudokuBoard *m_board;
    int m_index;
    QPoint *m_cell;
    int *m_value;
    QMap<QPoint, QList<int> > *m_values;
    QSemaphore *m_mutex;
    bool m_useThread;
};

}

#endif // GRIDBOARD_P_H
