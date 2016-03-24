#ifndef GRIDBOARD_H
#define GRIDBOARD_H

//Table widget used for displaying a Sudoku grid board

#include <QColor>
#include <QTableView>

namespace CIS5603
{

class GridBoardPrivate;
class GridModel;

class GridBoard : public QTableView
{
    Q_OBJECT
public:
    explicit GridBoard(QWidget *parent = 0);
    ~GridBoard();

    GridModel *girdModel();

    QSize aspectRatio() const;

    //Makes the table widget always square
    virtual int heightForWidth(int w) const;
    virtual bool hasHeightForWidth() const;

    int value(int row, int column) const;
    int value(const QPoint &pos) const;

    int rows() const;
    int columns() const;
    void setSize(int rows, int columns);

    //Set 1 or 2 cells with highlighted foreground color
    void setHighlight(const QPoint &p1, const QPoint &p2);
    void setHighlight(int row1, int column1, int row2, int column2);

    //Set background color for cell
    void setBackgroundColor(const QPoint &pos, const QColor &color);
    void setBackgroundColor(int row, int column, const QColor &color);

signals:
    void valueChanged(int row, int column, int oldValue, int newValue);

public slots:
    void setAspectRatio(const QSize &size);
    inline void setAspectRatio(int w, int h) { setAspectRatio(QSize(w, h)); }

    //Set value
    void setValue(int row, int column, int value);
    inline void setValue(const QPoint &pos, int value) { setValue(pos.x(), pos.y(), value); }

    //Empty all values
    void clearContents();

    //Clear the whole table
    void clear();

protected:
    virtual void resizeEvent(QResizeEvent *event); //Maintain square aspect ratio
    virtual void leaveEvent(QEvent *event); //Clear selection
    virtual void showEvent(QShowEvent *event);

private slots:
    void onValueChanged(const QModelIndex &index, int oldValue, int newValue);

private:
    GridBoardPrivate *m_private;
};

}

#endif // GRIDBOARD_H
