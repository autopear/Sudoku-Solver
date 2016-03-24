#ifndef SUDOKUBOARD_P_H
#define SUDOKUBOARD_P_H

#include <QColor>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QPolygon>
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
    {
        int c = qMin(p1.size(), p2.size());
        for (int i=0; i<c; i++)
        {
            QPoint pp1 = p1.at(i);
            QPoint pp2 = p2.at(i);
            if (pp1 < pp2)
                return true;
            else if (pp2 < pp1)
                return false;
            else
                continue;
        }
        if (p1.size() > c)
            return false;
        else if (p2.size() > c)
            return true;
        else
            return false;
    }
}

namespace CIS5603
{

class SudokuBoardPrivate
{
    friend class SudokuBoard;
protected:
    SudokuBoardPrivate();
    ~SudokuBoardPrivate();

    //Return 0 for success, -1 for any error, 1 for success, but is the same as the current configuration
    int loadFromFile(const QString &file, QString *error);

    //Sort all cells in a block in ascending order.
    //No duplicates are allowed
    //cell(x1, y1) is smaller than cell(x2, y2) if:
    //    x1 < x2
    //or  x1 == x2 and y1 < y2
    static void sortBlock(QPolygon *block);

    //Sort all blocks in ascending order
    //No duplicates are allowed
    //block1 is smaller than block2
    //if:
    //    the first cell in block1 is smaller than the first cell in block2
    //else:
    //    compare the second cells of both blocks
    static void sortBlocks(QList<QPolygon> *blocks);

    QString name;
    int rows;
    int columns;
    int min;
    int max;    
    QList<QPolygon> blocks;
    QMap<QPoint, QPolygon> blockMap;
    QList<QColor> colors;
};

}

#endif // SUDOKUBOARD_P_H
