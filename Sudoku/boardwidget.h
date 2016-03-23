#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

//Table widget used for displaying a Sudoku grid board

#include <QTableWidget>

namespace CIS5603
{

class BoardWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);
    ~BoardWidget();

    //Makes the table widget always square
    virtual int heightForWidth(int w) const;
    virtual bool hasHeightForWidth() const;

    //
    void scaleToFit(QTableWidgetItem *tableItem);
    inline void scaleToFit(int row, int column) { scaleToFit(item(row, column)); }

signals:
    void valueChanged(const QPoint &p, QString value);
    void valueChanged(int row, int column, QString value);

public slots:
    void emptyAllCells(); //Clear contents, but keep cells

protected:
    virtual void resizeEvent(QResizeEvent *event); //Maintain square aspect ratio
    virtual void leaveEvent(QEvent *event); //Clear selection

private slots:
    void onItemChanged(QTableWidgetItem *item);
};

}

#endif // BOARDWIDGET_H
