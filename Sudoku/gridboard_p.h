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
                               GridModel *model,
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
    GridModel *m_model;
    QSemaphore *m_mutex;
    QList<int> *m_values;
};

class BlockValuesChecker : public QThread
{
    Q_OBJECT
public:
    explicit BlockValuesChecker(const QPolygon &block,
                                GridModel *model,
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
    GridModel *m_model;
    QSemaphore *m_mutex;
    QList<int> *m_values;
};

class BestCellFinder : public QThread
{
    Q_OBJECT
public:
    explicit BestCellFinder(int index,
                            QPoint *cell,
                            int *value,
                            GridBoard *board,
                            QSemaphore *mutex,
                            bool useThread,
                            QObject *parent = 0);
    ~BestCellFinder() { }

public slots:
    void search();

protected:
    void run() { search(); }

private:
    int m_index;
    QPoint *m_cell;
    int *m_value;
    GridBoard *m_board;
    QSemaphore *m_mutex;
    bool m_useThread;
};

}

#endif // GRIDBOARD_P_H
