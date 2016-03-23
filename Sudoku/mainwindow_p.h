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

class BoardWidget;
class SudokuBoard;

class MainWindowPrivate
{
    friend class MainWindow;
protected:
    MainWindowPrivate();
    ~MainWindowPrivate();

    int loadBoards();

    QList<SudokuBoard *> boards;

    SudokuBoard *currentBoard;
    QList<QList<int> > values;

    MainWindow *parent;

    QWidget *mainWidget;

    QLabel *labelBoard;
    QComboBox *boxBoard;

    QLabel *labelAlgorithm;
    QComboBox *boxAlgorithm;

    QPushButton *buttonPreset;

    QPushButton *buttonInitialize;

    QPushButton *buttonAbout;

    BoardWidget *boardWidget;
    bool shown;
    int boardLeft, boardTop, boardBottom, boardRight;

    QLabel *labelTitleTotalTime;
    QLabel *labelTotalTime;

    QLabel *labelTitleStepTime;
    QLabel *labelStepTime;

    QPushButton *buttonNext;
    QPushButton *buttonEnd;
};

}

#endif // MAINWINDOW_P_H
