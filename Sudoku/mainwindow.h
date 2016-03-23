#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

namespace CIS5603
{

class MainWindowPrivate;
class SudokuBoard;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    SudokuBoard *currentBoard();

    //Return a 2-D arrays of values
    QList<QList<int> > values() const;

    //Return a list of values for the row, or empty list if the row number is invalid
    QList<int> rowValues(int row) const;

    //Return a list of values for the column, or empty list if the column number is invalid
    QList<int> columnValues(int column) const;

    //Return the value of a given cell, -1 is returned if the row or column is invalid, or the value is undefined
    int value(int row, int column) const;
    inline int value(const QPoint &pos) const { return value(pos.x(), pos.y()); }

public slots:
    //Reset all values to empty (undefined)
    void initialize();

    //Load .sdk file
    bool presetFromFile(const QString &file, bool showError = true);

    //Set a value for a given cell, this has no effect if row or column is invalid. Set value to -1 to set it undefined.
    bool setValue(int value, int row, int column);
    inline bool setValue(int value, const QPoint &pos) { return setValue(value, pos.x(), pos.y()); }

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

    //Compute the value for the next cell
    void nextStep();

    //Repeat computation until all values are set
    void goToEnd();

    //Stop the goToEnd() process, so user can compute values one by one
    void stop();

private:
    MainWindowPrivate *m_private;
};

}

#endif // MAINWINDOW_H
