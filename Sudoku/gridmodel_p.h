#ifndef GRIDMODEL_P_H
#define GRIDMODEL_P_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QModelIndex>

namespace CIS5603
{

class GridModelPrivate
{
    friend class GridModel;
protected:
    GridModelPrivate();
    ~GridModelPrivate();

    int rows;
    int columns;

    QMap<QModelIndex, QColor> colorsMap;

    QModelIndex highlight1;
    QModelIndex highlight2;

    QFont font;

    int **values;
};

}

#endif // GRIDMODEL_P_H
