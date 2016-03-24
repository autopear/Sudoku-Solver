#include <QFont>
#include <QFontMetrics>
#include <QSize>
#include "gridmodel_p.h"
#include "gridmodel.h"

using namespace CIS5603;

GridModel::GridModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_private(new GridModelPrivate())
{
}

GridModel::~GridModel()
{
    delete m_private;
}

int GridModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_private->rows;
}

int GridModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_private->columns;
}

int GridModel::value(int row, int column) const
{
    if (row < m_private->rows && row > -1 && column < m_private->columns && column > -1 && m_private->values)
        return m_private->values[row][column];
    else
        return -1;
}

void GridModel::setValue(int row, int column, int value)
{
    if (row < m_private->rows && row > -1 && column < m_private->columns && column > -1 && m_private->values)
    {
        if (value > -1 && value != m_private->values[row][column])
        {
            m_private->values[row][column] = value;
            emit dataChanged(index(row, column), index(row, column), QVector<int>() << Qt::DisplayRole);
        }
    }
}

const QFont &GridModel::font() const
{
    return m_private->font;
}

void GridModel::setFont(const QFont &font)
{
    m_private->font = font;
}

void GridModel::setSize(int rows, int columns)
{
    if (rows < 0 || columns < 0)
        return;

    if (rows == 0 || columns == 0)
    {
        if (0 == m_private->rows && 0 == m_private->columns)
            return;
        else
        {
            m_private->rows = 0;
            m_private->columns = 0;

            if (m_private->values)
            {
                delete m_private->values;
                m_private->values = 0;
            }
            emit layoutChanged();
            return;
        }
    }

    if (rows == m_private->rows && columns == m_private->columns)
        return;

    if (m_private->values)
        delete m_private->values;

    m_private->values = (int **)malloc(sizeof(int *) * rows);
    for (int i=0; i<rows; i++)
    {
        int *r = (int *)malloc(sizeof(int) * columns);
        for (int j=0; j<columns; j++)
            r[j] = 0;
        m_private->values[i] = r;
    }

    m_private->rows = rows;
    m_private->columns = columns;

    emit layoutChanged();
}

QVariant GridModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.row() > -1 && index.column() > -1)
        {
            int v = m_private->values[index.row()][index.column()];
            if (v > 0)
                return QVariant(QString::number(v, 10));
        }
    }
    else if (role == Qt::FontRole)
        return QVariant(m_private->font);
    else if (role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignCenter);
    else if (role == Qt::EditRole)
        return data(index, Qt::DisplayRole);
    else if (role == Qt::BackgroundRole)
        return QVariant(m_private->colorsMap.value(index, QColor(Qt::white)));
    else if (role == Qt::ForegroundRole)
    {
        if (index.isValid() && (index == m_private->highlight1 || index == m_private->highlight2))
            return QVariant(QColor(Qt::red));
        else
            return QVariant(QColor(Qt::black));
    }
    else
    {
    }
    return QVariant();
}

QVariant GridModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(QString::number(section + 1, 10));
    else if (role == Qt::FontRole)
    {
        QFont font = qvariant_cast<QFont>(QAbstractTableModel::headerData(section, orientation, Qt::FontRole));
        font.setBold(true);
        return QVariant(font);
    }
    else if (role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignCenter);
    else if (role == Qt::EditRole)
        return QVariant("sdfsf");
    else
    {
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

bool GridModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (index.row() > -1 && index.column() > -1)
        {
            int oldValue = m_private->values[index.row()][index.column()];
            if (value.toString().trimmed().isEmpty())
            {
                if (oldValue > 0)
                {
                    m_private->values[index.row()][index.column()] = 0;
                    emit valueChanged(index, oldValue, 0);
                    return true;
                }
            }
            else
            {
                bool ok;
                int newValue = value.toInt(&ok);
                if (ok &&
                        (newValue > -1 && newValue <= qMax(m_private->columns, m_private->rows)) &&
                        oldValue != newValue)
                {
                    m_private->values[index.row()][index.column()] = newValue;
                    emit valueChanged(index, oldValue, newValue);
                    return true;
                }
            }
        }
        return false;
    }
    return QAbstractTableModel::setData(index, value, role);
}

Qt::ItemFlags GridModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

const QColor &GridModel::backgroundColor(const QModelIndex &index) const
{
    return m_private->colorsMap.value(index, QColor());
}

void GridModel::setBackgroundColor(const QModelIndex &index, const QColor &color)
{
    if (index.isValid() &&
            index.row() > -1 && index.row() < m_private->rows &&
            index.column() > -1 && index.column() < m_private->columns)
        m_private->colorsMap.insert(index, color);
}

QModelIndex GridModel::highlightIndex1() const
{
    return m_private->highlight1;
}

QModelIndex GridModel::highlightIndex2() const
{
    return m_private->highlight2;
}

void GridModel::setHighlightIndex(const QModelIndex &index1, const QModelIndex &index2)
{
    if (index1.isValid() &&
            index1.row() > -1 && index1.row() < m_private->rows &&
            index1.column() > -1 && index1.column() < m_private->columns &&
            index2.isValid() &&
            index2.row() > -1 && index2.row() < m_private->rows &&
            index2.column() > -1 && index2.column() < m_private->columns &&
            index1 != index2)
    {
        m_private->highlight1 = index1;
        m_private->highlight2 = index2;
    }
}

void GridModel::setHighlightIndex(const QModelIndex &index)
{
    if (index.isValid() &&
            index.row() > -1 && index.row() < m_private->rows &&
            index.column() > -1 && index.column() < m_private->columns)
    {
        m_private->highlight1 = index;
        m_private->highlight2 = QModelIndex();
    }
}

bool GridModel::hasHighlights() const
{
    return m_private->highlight1.isValid() || m_private->highlight2.isValid();
}

void GridModel::removeHighlights()
{
    m_private->highlight1 = QModelIndex();
    m_private->highlight2 = QModelIndex();
}

GridModelPrivate::GridModelPrivate()
{
    rows = 0;
    columns = 0;

    values = 0;

    colorsMap = QMap<QModelIndex, QColor>();

    highlight1 = QModelIndex();
    highlight2 = QModelIndex();
}

GridModelPrivate::~GridModelPrivate()
{
    if (values)
    {
        delete values;
        values = 0;
    }
}
