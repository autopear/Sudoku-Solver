#ifndef SUDOKUBOARD_P_H
#define SUDOKUBOARD_P_H

#include <QMap>
#include <QPoint>
#include <QPolygon>
#include <QSize>
#include <QString>
#include "sudokuboard.h"

static bool operator <(const QPoint &p1, const QPoint &p2)
{
    if (p1.x() == p2.x())
        return p1.y() < p2.y();
    else
        return p1.x() < p2.x();
}

static bool operator <(const QPolygon &p1, const QPolygon &p2)
{
    if (p1.isEmpty() && p2.isEmpty())
        return false;
    else if (p1.isEmpty() && !p2.isEmpty())
        return true;
    else if (!p1.isEmpty() && p2.isEmpty())
        return false;
    else
        return p1.first() < p2.first();
}

namespace CIS5603
{

class SudokuBoardPrivate
{
    friend class SudokuBoard;
protected:
    SudokuBoardPrivate();
    ~SudokuBoardPrivate();

    int loadFromFile(const QString &file, QString *error);

    static void sortBlock(QPolygon *block);
    static void sortBlocks(QList<QPolygon> *blocks);

    QString name;
    QSize size;
    int min;
    int max;    
    QList<QPolygon> blocks;
    QMap<QPoint, QPolygon> blockMap;
};

}

#endif // SUDOKUBOARD_P_H
