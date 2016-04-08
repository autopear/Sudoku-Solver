#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include <QList>
#include "mainwindow.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QWidget;
class QElapsedTimer;
class QTimer;

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
    void blockWidgets(bool block);

    static QString msToString(qint64 ms);

    QList<SudokuBoard *> boards;

    SudokuBoard *currentBoard;

    MainWindow *parent;

    QWidget *mainWidget;

    QLabel *labelBoard;
    QComboBox *boxBoard;

    QLabel *labelAlgorithm;
    QComboBox *boxAlgorithm;

    QCheckBox *checkMultiThread;

    QPushButton *buttonPreset;

    QPushButton *buttonInitialize;

    QPushButton *buttonAbout;

    QWidget *blankTopLeft;
    QWidget *blankBottomRight;
    GridBoard *boardWidget;
    bool shown;
    int boardLeft, boardTop, boardBottom, boardRight;

    QLabel *labelTitleTime;
    QLabel *labelTime;

    QPushButton *buttonSolve;

    SudokuSolver *solver;

    QString lastPreset;

    QTimer *timer;
    QElapsedTimer *elapsedTimer;
};

}

#endif // MAINWINDOW_P_H
