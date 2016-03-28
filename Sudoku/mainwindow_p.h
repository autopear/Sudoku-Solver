#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include <QList>
#include "mainwindow.h"

class QComboBox;
class QLabel;
class QPushButton;
class QWidget;

namespace CIS5603
{

class GridBoard;
class SudokuBoard;
class SudokuSolver;

class MainWindowPrivate
{
    friend class MainWindow;
protected:
    MainWindowPrivate();
    ~MainWindowPrivate();

    int loadBoards(); //Return the number of loaded boards, or -1 if any error happens

    void updateWidgets();

    static QString msToString(qint64 ms);

    QList<SudokuBoard *> boards;

    SudokuBoard *currentBoard;

    MainWindow *parent;

    QWidget *mainWidget;

    QLabel *labelBoard;
    QComboBox *boxBoard;

    QLabel *labelAlgorithm;
    QComboBox *boxAlgorithm;

    QPushButton *buttonPreset;

    QPushButton *buttonInitialize;

    QPushButton *buttonAbout;

    QWidget *blankTopLeft;
    QWidget *blankBottomRight;
    GridBoard *boardWidget;
    bool shown;
    int boardLeft, boardTop, boardBottom, boardRight;

    QLabel *labelTitleTotalTime;
    QLabel *labelTotalTime;

    QLabel *labelTitleStepTime;
    QLabel *labelStepTime;

    QPushButton *buttonNext;
    QPushButton *buttonEnd;

    SudokuSolver *solver;
};

}

#endif // MAINWINDOW_P_H
