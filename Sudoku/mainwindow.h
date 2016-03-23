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

    QList<QList<int> > values() const;
    QList<int> rowValues(int row) const;
    QList<int> columnValues(int column) const;
    int value(int row, int column) const;
    inline int value(const QPoint &pos) const { return value(pos.x(), pos.y()); }

public slots:
    void initialize();
    bool presetFromFile(const QString &file, bool showError = true);
    bool setValue(int value, int row, int column);
    inline bool setValue(int value, const QPoint &pos) { return setValue(value, pos.x(), pos.y()); }
    bool validateRow(int row, QPoint *p1, QPoint *p2);
    bool validateColumn(int column, QPoint *p1, QPoint *p2);
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
    void nextStep();
    void goToEnd();
    void stop();

private:
    MainWindowPrivate *m_private;
};

}

#endif // MAINWINDOW_H
