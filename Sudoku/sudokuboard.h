#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

//Model for Sudoku board

#include <QMap>
#include <QObject>
#include <QPoint>
#include <QPolygon>
#include <QSize>

namespace CIS5603
{

class SudokuBoardPrivate;

class SudokuBoard : public QObject
{
    Q_OBJECT
public:
    explicit SudokuBoard(QObject *parent = 0);
    explicit SudokuBoard(const QString &file, QObject *parent = 0);
    explicit SudokuBoard(SudokuBoard *other, QObject *parent = 0);
    ~SudokuBoard();

    bool isValid() const;

    QString name() const;

    QSize size() const; //Board size
    inline int width() const { return size().width(); }
    inline int height() const { return size().height(); }

    int minimum() const; //Minimum value
    int maximum() const; //Maximum value

    QList<QPolygon> blocks() const; //All inner blocks

    //Return the block that the cell belongs to, or empty block if the cell does not belong to any block
    QPolygon findBlock(const QPoint &pos);
    inline QPolygon findBlock(int x, int y) { return findBlock(QPoint(x, y)); }

    bool saveToFile(const QString &file, QString *error = 0);

signals:
    void configurationChanged();
    void nameChanged(const QString &oldName);
    void widthChanged(int oldWidth);
    void heightChanged(int oldHeight);
    void sizeChanged(const QSize &oldSize);
    void minimumChanged(int oldMin);
    void maximumChanged(int oldMax);
    void rangeChanged(int oldMin, int oldMax);
    void blocksChanged(const QList<QPolygon> &oldBlocks);

public slots:
    bool loadFromFile(const QString &file, QString *error = 0);
    void setName(const QString &name);
    void setWidth(int w);
    void setHeight(int h);
    void setSize(const QSize &size);
    inline void setSize(int w, int h) { setSize(QSize(w, h)); }
    void setMinimum(int min);
    void setMaximum(int max);
    void setRange(int min, int max);
    void setBlocks(const QList<QPolygon> &blocks);

private:
    SudokuBoardPrivate *m_private;
};

}

#endif // SUDOKUBOARD_H
