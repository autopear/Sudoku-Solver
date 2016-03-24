#ifndef GRIDBOARD_P_H
#define GRIDBOARD_P_H

#include "gridboard.h"

class QLabel;

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

}

#endif // GRIDBOARD_P_H
