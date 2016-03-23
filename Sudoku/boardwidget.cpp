#include <QHeaderView>
#include "boardwidget.h"

using namespace CIS5603;

BoardWidget::BoardWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setFrameShape(QFrame::Box);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setTextElideMode(Qt::ElideNone);

    horizontalHeader()->setMinimumSectionSize(0);
    verticalHeader()->setMinimumSectionSize(0);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(onItemChanged(QTableWidgetItem*)));
}

BoardWidget::~BoardWidget()
{

}

int BoardWidget::heightForWidth(int w) const
{
    return w;
}

bool BoardWidget::hasHeightForWidth() const
{
    return true;
}

void BoardWidget::scaleToFit(QTableWidgetItem *tableItem)
{
    if (!tableItem)
        return;

    int w = horizontalHeader()->sectionSize(tableItem->row());
    int h = verticalHeader()->sectionSize(tableItem->column());

    if (!tableItem->text().isEmpty())
    {
        QFont font = tableItem->font();

        for (int i=1; ; i++)
        {
            font.setPixelSize(i);
            QFontMetrics fm(font);
            QRect rect = fm.boundingRect(tableItem->text());
            if (rect.width() > w - 4 || rect.height() > h - 4)
            {
                font.setPixelSize(i - 1);
                break;
            }
        }
        tableItem->setFont(font);
    }
}

void BoardWidget::emptyAllCells()
{
    for (int i=0; i<rowCount(); i++)
    {
        for (int j=0; j<columnCount(); j++)
        {
            QTableWidgetItem *tableItem = item(i, j);
            if (tableItem)
                tableItem->setText(QString());
        }
    }
}

void BoardWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);
    for (int i=0; i<rowCount(); i++)
    {
        for (int j=0; j<columnCount(); j++)
            scaleToFit(i, j);
    }
}

void BoardWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    clearSelection();
}

void BoardWidget::onItemChanged(QTableWidgetItem *item)
{
    if (item)
    {
        scaleToFit(item);
        emit valueChanged(item->row(), item->column(), item->text());
        emit valueChanged(QPoint(item->row(), item->column()), item->text());
    }
}
