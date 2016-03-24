#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QAbstractTableModel>

namespace CIS5603
{

class GridModelPrivate;

class GridModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GridModel(QObject *parent = 0);
    ~GridModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void setSize(int rows, int columns);

    int value(int row, int column) const;
    void setValue(int row, int column, int value);

    const QFont &font() const;
    void setFont(const QFont &font);

    const QColor &backgroundColor(const QModelIndex &index) const;
    inline const QColor &backgroundColor(int row, int column) const { return backgroundColor(index(row, column)); }
    void setBackgroundColor(const QModelIndex &index, const QColor &color);
    inline void setBackgroundColor(int row, int column, const QColor &color) { setBackgroundColor(index(row, column), color); }

    QModelIndex highlightIndex1() const;
    QModelIndex highlightIndex2() const;
    void setHighlightIndex(const QModelIndex &index1, const QModelIndex &index2);
    void setHighlightIndex(const QModelIndex &index);
    inline void setHighlightIndex(int row1, int column1, int row2, int column2) { setHighlightIndex(index(row1, column1), index(row2, column2)); }
    inline void setHighlightIndex(int row, int column) { setHighlightIndex(index(row, column)); }

    bool hasHighlights() const;
    void removeHighlights();

signals:
    void valueChanged(const QModelIndex &index, int oldValue, int newValue);

private:
    GridModelPrivate *m_private;
};

}

#endif // GRIDMODEL_H
