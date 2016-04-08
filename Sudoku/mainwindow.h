#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

namespace CIS5603
{

class GridBoard;
class MainWindowPrivate;
class SudokuBoard;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *sharedInstance();

    SudokuBoard *currentBoard();

    GridBoard *gridBoard();

    bool multithreadingEnabled() const;

public slots:
    //Reset all values to empty (undefined)
    void initialize();

    //Load .sdk file
    bool presetFromFile(const QString &file, bool showError = true);

    //Set a value for a given cell, this has no effect if row or column is invalid. Set value to -1 to set it undefined.
    bool setValue(int row, int column, int value);
    inline bool setValue(const QPoint &pos, int value) { return setValue(pos.x(), pos.y(), value); }

    //Validate the given row values.
    //If a cell has invalid value, only p1 will be set.
    //If two cells have the same non-empty value, both p1 and p2 will be set.
    bool validateRow(int row, QPoint *p1, QPoint *p2);

    //Validate the given column values.
    //If a cell has invalid value, only p1 will be set.
    //If two cells have the same non-empty value, both p1 and p2 will be set.
    bool validateColumn(int column, QPoint *p1, QPoint *p2);

    //Validate all cells in the given block.
    //If a cell has invalid value, only p1 will be set.
    //If two cells have the same non-empty value, both p1 and p2 will be set.
    bool validateBlock(const QPolygon &block, QPoint *p1, QPoint *p2);

    //Adjust board geometry to maintain aspect ratio and alignment
    void adjustBoard();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void onBoardSelected(int index);
    void onAlgorithmSelected(int index);
    void showAbout();
    void loadPreset();

    void solveSudoku();

    //Validate values from user input
    void onValueChanged(int row, int column, int oldValue, int newValue);

    void onFinished();
    void onTerminated(const QString &message);

    void updateTime();

private:
    MainWindowPrivate *m_private;
};

}

#endif // MAINWINDOW_H
