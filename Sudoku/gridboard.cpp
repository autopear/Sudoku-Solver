#include <QLabel>
#include <QHeaderView>
#include <QFont>
#include <QHBoxLayout>
#include <QSemaphore>
#include "gridmodel.h"
#include "mainwindow.h"
#include "sudokuboard.h"
#include "sudokuboard_p.h"
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

    setStyleSheet("* { gridline-color: black; }");
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

QList<int> GridBoard::availableValues(int row, int column, bool multiThread)
{
    if (row > -1 && row < m_private->model->rowCount() && column > -1 && column < m_private->model->columnCount())
    {
        int currentValue = m_private->model->value(row, column);
        if (currentValue > 0)
            return QList<int>(); //Already defined

        int rows = m_private->model->rowCount();
        int columns = m_private->model->columnCount();

        QSemaphore *mutex = multiThread ? new QSemaphore(1) : 0;
        QList<int> usedValues;

        LineValuesChecker *rowChecker = new LineValuesChecker(row,
                                                              true,
                                                              m_private->model,
                                                              mutex,
                                                              &usedValues,
                                                              this);
        LineValuesChecker *colChecker = new LineValuesChecker(column,
                                                              false,
                                                              m_private->model,
                                                              mutex,
                                                              &usedValues,
                                                              this);

        QList<BlockValuesChecker *> blockCheckers;
        MainWindow *mw = qobject_cast<MainWindow *>(parentWidget());
        if (mw && mw->currentBoard())
        {
            QList<QPolygon> blocks = mw->currentBoard()->findBlocks(row, column);
            foreach (QPolygon block, blocks)
                blockCheckers.append(new BlockValuesChecker(block,
                                                            m_private->model,
                                                            mutex,
                                                            &usedValues,
                                                            this));
        }

        if (multiThread)
        {
            rowChecker->start();
            colChecker->start();
            foreach (BlockValuesChecker *checker, blockCheckers)
                checker->start();

            rowChecker->wait();
            colChecker->wait();
            foreach (BlockValuesChecker *checker, blockCheckers)
                checker->wait();

            delete mutex;
        }
        else
        {
            rowChecker->check();
            colChecker->check();
            foreach (BlockValuesChecker *checker, blockCheckers)
                checker->check();
        }

        delete rowChecker;
        delete colChecker;
        foreach (BlockValuesChecker *checker, blockCheckers)
            delete checker;

        QList<int> availables;
        int max = qMax(rows, columns);
        for (int i=1; i<=max; i++)
        {
            if (!usedValues.contains(i))
                availables.append(i);
        }
        return availables;
    }

    return QList<int>();
}

QList<int> GridBoard::availableValues(const QPoint &pos, bool multiThread)
{
    return availableValues(pos.x(), pos.y(), multiThread);
}

QPoint GridBoard::getBestCell(int *value, QMap<QPoint, QList<int> > *values, bool multiThread)
{
    int rows = m_private->model->rowCount();
    int cols = m_private->model->columnCount();
    if (rows < 1 || cols < 1)
        return QPoint(-1, -1);

    int numThreads = (rows % 2 == 0) ? (rows / 2) : ((rows + 1) / 2);

    QPoint bestCell(-1, -1);
    int bestValue = 0;
    QMap<QPoint, QList<int> > bestValues;
    bool useThread = (multiThread && numThreads > 1);
    QSemaphore *mutex = useThread ? new QSemaphore(1) : 0;
    QList<BestCellFinder *> finders;
    for (int i=0; i<numThreads; i++)
    {
        BestCellFinder *finder = new BestCellFinder(i,
                                                    &bestCell,
                                                    &bestValue,
                                                    &bestValues,
                                                    const_cast<GridBoard *>(this),
                                                    mutex,
                                                    multiThread,
                                                    this);
        finders.append(finder);

        if (useThread)
            finder->start();
        else
            finder->search();
    }

    if (useThread)
    {
        foreach (BestCellFinder *finder, finders)
            finder->wait();
    }

    if (mutex)
        delete mutex;
    foreach (BestCellFinder *finder, finders)
        delete finder;

    if (value)
        *value = bestValue;
    if (values)
        *values = bestValues;

    return bestCell;
}

void GridBoard::setValue(int row, int column, int value)
{
    m_private->model->setValue(row, column, value);
}

void GridBoard::setValue(const QPoint &pos, int value)
{
    setValue(pos.x(), pos.y(), value);
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

void GridBoard::setAspectRatio(int w, int h)
{
    setAspectRatio(QSize(w, h));
}

void GridBoard::setHighlight(const QPoint &p1, const QPoint &p2)
{
    setHighlight(p1.x(), p1.y(), p2.x(), p2.y());
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
    m_private->model->removeHighlights();
    for (int i=0; i<m_private->model->rowCount(); i++)
    {
        for (int j=0; j<m_private->model->columnCount(); j++)
            m_private->model->setValue(i, j, 0);
    }
}

void GridBoard::clear()
{
    m_private->model->removeHighlights();
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
            if (i < 2)
                return;

            font.setPixelSize(i - 1);
            model->setFont(font);
            return;
        }
    }
}

LineValuesChecker::LineValuesChecker(int line,
                                     bool isRow,
                                     GridModel *model,
                                     QSemaphore *mutex,
                                     QList<int> *usedValues,
                                     QObject *parent) :
    QThread(parent),
    m_line(line),
    m_isRow(isRow),
    m_model(model),
    m_mutex(mutex),
    m_values(usedValues)
{
}

void LineValuesChecker::check()
{
    if (!m_model || m_line < 0)
        return;

    int rows = m_model->rowCount();
    int cols = m_model->columnCount();

    if (rows < 1 || cols < 1)
        return;

    if (m_isRow)
    {
        if (m_line >= rows)
            return;

        for (int i=0; i<cols; i++)
        {
            int v = m_model->value(m_line, i);
            if (v > 0)
            {
                if (m_mutex)
                    m_mutex->acquire();
                if (!m_values->contains(v))
                    m_values->append(v);
                if (m_mutex)
                    m_mutex->release();
            }
        }
    }
    else
    {
        if (m_line >= cols)
            return;

        for (int i=0; i<rows; i++)
        {
            int v = m_model->value(i, m_line);
            if (v > 0)
            {
                if (m_mutex)
                    m_mutex->acquire();
                if (!m_values->contains(v))
                    m_values->append(v);
                if (m_mutex)
                    m_mutex->release();
            }
        }
    }
}

BlockValuesChecker::BlockValuesChecker(const QPolygon &block,
                                       GridModel *model,
                                       QSemaphore *mutex,
                                       QList<int> *usedValues,
                                       QObject *parent) :
    QThread(parent),
    m_block(block),
    m_model(model),
    m_mutex(mutex),
    m_values(usedValues)
{
}

void BlockValuesChecker::check()
{
    if (!m_model)
        return;

    if (m_model->rowCount() < 1 || m_model->columnCount() < 1)
        return;

    foreach (QPoint p, m_block)
    {
        int v = m_model->value(p.x(), p.y());
        if (v > 0)
        {
            if (m_mutex)
                m_mutex->acquire();
            if (!m_values->contains(v))
                m_values->append(v);
            if (m_mutex)
                m_mutex->release();
        }
    }
}

BestCellFinder::BestCellFinder(int index,
                               QPoint *cell,
                               int *value,
                               QMap<QPoint, QList<int> > *values,
                               GridBoard *board,
                               QSemaphore *mutex,
                               bool useThread,
                               QObject *parent) :
    QThread(parent),
    m_index(index),
    m_cell(cell),
    m_value(value),
    m_values(values),
    m_board(board),
    m_mutex(mutex),
    m_useThread(useThread)
{
}

void BestCellFinder::search()
{
    int rows = m_board->rows();
    int cols = m_board->columns();
    int max = qMin(m_index * 2 + 2, rows);

    for (int i=m_index*2; i<max; i++)
    {
        for (int j=0; j<cols; j++)
        {
            if (m_mutex)
                m_mutex->acquire();
            if (m_cell->x() > -1 && m_cell->y() > -1)
            {
                //Terminate, some thread has already found the best cell
                if (m_mutex)
                    m_mutex->release();
                return;
            }

            QList<int> availableValues = m_board->availableValues(i, j, m_useThread);
            if (availableValues.size() == 1)
            {
                m_cell->setX(i);
                m_cell->setY(j);
                if (m_value)
                    *m_value = availableValues.first();
                if (m_mutex)
                    m_mutex->release();
                return;
            }
            else if (availableValues.size() > 1)
            {
                if (m_values)
                    m_values->insert(QPoint(i, j), availableValues);
            }
            else
            {
            }

            if (m_mutex)
                m_mutex->release();
        }
    }
}
