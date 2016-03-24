#include <QLabel>
#include <QHeaderView>
#include <QFont>
#include <QHBoxLayout>
#include "gridmodel.h"
#include "gridboard_p.h"
#include "gridboard.h"

using namespace CIS5603;

GridBoard::GridBoard(QWidget *parent) :
    QTableView(parent),
    m_private(new GridBoardPrivate())
{
    m_private->board = this;

    m_private->label = new QLabel(tr("Please select a board..."), this);
    m_private->label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_private->label);
    setLayout(layout);

    m_private->model = new GridModel(this);
    setModel(m_private->model);
    connect(m_private->model, SIGNAL(valueChanged(QModelIndex,int,int)),
            this, SLOT(onValueChanged(QModelIndex,int,int)));

    setFrameShape(QFrame::Box);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setTextElideMode(Qt::ElideNone);

    horizontalHeader()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    verticalHeader()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    horizontalHeader()->setMinimumSectionSize(0);
    verticalHeader()->setMinimumSectionSize(0);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

GridBoard::~GridBoard()
{
    delete m_private;
}

GridModel *GridBoard::girdModel()
{
    return m_private->model;
}

QSize GridBoard::aspectRatio() const
{
    return m_private->size;
}

int GridBoard::heightForWidth(int w) const
{
    return w * m_private->size.height() / m_private->size.width();
}

bool GridBoard::hasHeightForWidth() const
{
    return true;
}

int GridBoard::value(int row, int column) const
{
    return m_private->model->value(row, column);
}

int GridBoard::value(const QPoint &pos) const
{
    return m_private->model->value(pos.y(), pos.x());
}

void GridBoard::setValue(int row, int column, int value)
{
    m_private->model->setData(m_private->model->index(row, column), value);
}

int GridBoard::rows() const
{
    return m_private->model->rowCount();
}

int GridBoard::columns() const
{
    return m_private->model->columnCount();
}

void GridBoard::setSize(int rows, int columns)
{
    m_private->model->setSize(rows, columns);
    m_private->label->setVisible(m_private->model->rowCount() == 0);

    QFont font(m_private->model->font());
    int mw = 0;
    QFontMetrics mt(font);
    for (int i=1; i<=qMax(rows, columns); i++)
    {
        QString str = QString::number(i, 10);
        int nw = mt.width(str);
        if (nw > mw)
        {
            mw = nw;
            m_private->maxWidthNum = i;
        }
    }

    m_private->adjustFont();
}

void GridBoard::setAspectRatio(const QSize &size)
{
    if (size.isValid() && (size.width() * m_private->size.height() != size.height() * m_private->size.width()))
        m_private->size = size;
}

void GridBoard::setHighlight(const QPoint &p1, const QPoint &p2)
{
    setHighlight(p1.y(), p1.x(), p2.y(), p2.x());
}

void GridBoard::setHighlight(int row1, int column1, int row2, int column2)
{
    QModelIndex idx1 = m_private->model->index(row1, column1);
    QModelIndex idx2 = m_private->model->index(row2, column2);

    if (idx1.isValid() && idx2.isValid())
    {
        if (idx1 == idx2)
            m_private->model->setHighlightIndex(idx1);
        else
            m_private->model->setHighlightIndex(idx1, idx2);
    }
    else if (idx1.isValid())
        m_private->model->setHighlightIndex(idx1);
    else if (idx2.isValid())
        m_private->model->setHighlightIndex(idx2);
    else
        m_private->model->removeHighlights();
}

void GridBoard::setBackgroundColor(const QPoint &pos, const QColor &color)
{
    m_private->model->setBackgroundColor(pos.y(), pos.x(), color);
}

void GridBoard::setBackgroundColor(int row, int column, const QColor &color)
{
    m_private->model->setBackgroundColor(row, column, color);
}

void GridBoard::clearContents()
{
    for (int i=0; i<m_private->model->rowCount(); i++)
    {
        for (int j=0; j<m_private->model->columnCount(); j++)
            m_private->model->setValue(i, j, 0);
    }
}

void GridBoard::clear()
{
    m_private->model->setSize(0, 0);
}

void GridBoard::onValueChanged(const QModelIndex &index, int oldValue, int newValue)
{
    if (index.isValid())
        emit valueChanged(index.row(), index.column(), oldValue, newValue);
}

void GridBoard::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    m_private->adjustFont();
}

void GridBoard::leaveEvent(QEvent *event)
{
    QTableView::leaveEvent(event);
    clearSelection();
}

void GridBoard::showEvent(QShowEvent *event)
{
    QTableView::showEvent(event);
    if (m_private->model->font().pixelSize() < 1)
    {
        QFont f = font();
        f.setItalic(true);
        m_private->model->setFont(f);
    }
}

GridBoardPrivate::GridBoardPrivate()
{
    label = 0;
    size = QSize(1, 1);
    model = 0;
    maxWidthNum = 1;
}

GridBoardPrivate::~GridBoardPrivate()
{
    delete label;
    delete model;
}

void GridBoardPrivate::adjustFont()
{
    if (model->rowCount() < 1 || model->columnCount() < 1)
        return;

    int w = board->horizontalHeader()->sectionSize(0);
    int h = board->verticalHeader()->sectionSize(0);
    int s = qMin(w, h);

    QFont font(model->font());
    QString maxStr = QString::number(maxWidthNum, 10);
    for (int i=1; ;i++)
    {
        font.setPixelSize(i);
        QFontMetrics mt = QFontMetrics(font);

        if (qMax(mt.width(maxStr), mt.height()) > s - 8)
        {
            if (i == 0)
                return;

            font.setPixelSize(i - 1);
            model->setFont(font);
            return;
        }
    }
}
