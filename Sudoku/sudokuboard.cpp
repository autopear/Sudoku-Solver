#include <QFile>
#include "sudokuboard_p.h"
#include "sudokuboard.h"

using namespace CIS5603;

SudokuBoard::SudokuBoard(QObject *parent) :
    QObject(parent),
    m_private(new SudokuBoardPrivate())
{
}

SudokuBoard::SudokuBoard(const QString &file, QObject *parent) :
    QObject(parent),
    m_private(new SudokuBoardPrivate())
{
    m_private->loadFromFile(file, 0);
}

SudokuBoard::SudokuBoard(SudokuBoard *other, QObject *parent) :
    QObject(parent),
    m_private(new SudokuBoardPrivate())
{
    if (other)
    {
        m_private->name = other->name();
        m_private->size = other->size();
        m_private->min = other->minimum();
        m_private->max = other->maximum();
        m_private->blocks = other->blocks();
    }
}

SudokuBoard::~SudokuBoard()
{    
    delete m_private;
}

bool SudokuBoard::isValid() const
{
    if (m_private->name.isEmpty())
        return false;

    if (m_private->size.width() < 4 || m_private->size.height() < 4)
        return false;

    if (m_private->min < 1 || m_private->max <= m_private->min)
        return false;

    //Check blocks;
    QList<QPoint> tmp;
    foreach (QPolygon block, m_private->blocks)
    {
        if (block.isEmpty())
            return false;

        foreach (QPoint p, block)
        {
            if (tmp.contains(p))
                return false;
            else
                tmp.append(p);
        }
    }

    return true;
}

QString SudokuBoard::name() const
{
    return m_private->name;
}

QSize SudokuBoard::size() const
{
    return m_private->size;
}

int SudokuBoard::minimum() const
{
    return m_private->min;
}

int SudokuBoard::maximum() const
{
    return m_private->max;
}

QList<QPolygon> SudokuBoard::blocks() const
{
    return m_private->blocks;
}

QPolygon SudokuBoard::findBlock(const QPoint &pos)
{
    return m_private->blockMap.value(pos, QPolygon());
}

static QString stringFromPoint(const QPoint &p)
{
    return QString("(%1, %2)").arg(QString::number(p.x(), 10)).arg(QString::number(p.y(), 10));
}

static QString stringFromBlock(const QPolygon &blk)
{
    QStringList tmp;
    foreach (QPoint p, blk)
        tmp.append(stringFromPoint(p));
    return QString("[%1]").arg(tmp.join(", "));
}

static QString stringFromBlocks(const QList<QPolygon> &blks)
{
    QStringList tmp;
    foreach (QPolygon p, blks)
        tmp.append(stringFromBlock(p));
    return QString("%1;").arg(tmp.join(",\n"));
}

bool SudokuBoard::saveToFile(const QString &file, QString *error)
{
    QFile *sdk = new QFile(file);
    if (!sdk->open(QFile::WriteOnly | QFile::Truncate))
    {
        if (error)
            *error = sdk->errorString();
        return false;
    }

    if (!m_private->name.isEmpty())
        sdk->write(QString("NAME: %1\n").arg(m_private->name).toUtf8());
    sdk->write(QString("WIDTH: %1\n").arg(QString::number(m_private->size.width(), 10)).toUtf8());
    sdk->write(QString("HEIGHT: %1\n").arg(QString::number(m_private->size.height(), 10)).toUtf8());
    sdk->write(QString("MINIMUM: %1\n").arg(QString::number(m_private->min, 10)).toUtf8());
    sdk->write(QString("MAXIMUM: %1\n").arg(QString::number(m_private->max, 10)).toUtf8());
    if (!m_private->blocks.isEmpty())
    {
        sdk->write("BLOCKS:\n");
        sdk->write(stringFromBlocks(m_private->blocks).toUtf8());
    }
    sdk->close();
    delete sdk;
    if (error)
        error->clear();
    return true;
}

bool SudokuBoard::loadFromFile(const QString &file, QString *error)
{
    QString err;
    int ret = m_private->loadFromFile(file, &err);
    if (ret == 1)
    {
        emit configurationChanged();
        return true;
    }
    else if (ret == 0)
        return true;
    else
    {
        if (error)
            *error = err;
        return false;
    }
}

void SudokuBoard::setName(const QString &name)
{
    if (m_private->name.compare(name, Qt::CaseSensitive) != 0)
    {
        QString oldName = m_private->name;
        m_private->name = name;
        emit nameChanged(oldName);
        emit configurationChanged();
    }
}

void SudokuBoard::setWidth(int w)
{
    if (w != m_private->size.width())
    {
        QSize old = m_private->size;
        m_private->size.setWidth(w);
        emit widthChanged(old.width());
        emit sizeChanged(old);
        emit configurationChanged();
    }
}

void SudokuBoard::setHeight(int h)
{
    if (h != m_private->size.height())
    {
        QSize old = m_private->size;
        m_private->size.setHeight(h);
        emit heightChanged(old.height());
        emit sizeChanged(old);
        emit configurationChanged();
    }
}

void SudokuBoard::setSize(const QSize &size)
{
    if (size != m_private->size)
    {
        QSize old = m_private->size;
        bool wChanged = size.width() != m_private->size.width();
        bool hChanged = size.height() != m_private->size.height();
        m_private->size = size;
        if (wChanged)
            emit widthChanged(old.width());
        if (hChanged)
            emit heightChanged(old.height());
        emit sizeChanged(old);
        emit configurationChanged();
    }
}

void SudokuBoard::setMinimum(int min)
{
    if (min != m_private->min)
    {
        int old = m_private->min;
        m_private->min = min;
        emit minimumChanged(old);
        emit rangeChanged(old, m_private->max);
        emit configurationChanged();
    }
}

void SudokuBoard::setMaximum(int max)
{
    if (max != m_private->max)
    {
        int old = m_private->max;
        m_private->max = max;
        emit maximumChanged(max);
        emit rangeChanged(m_private->min, old);
        emit configurationChanged();
    }
}

void SudokuBoard::setRange(int min, int max)
{
    bool minChanged = min != m_private->min;
    bool maxChanged = max != m_private->max;

    if (minChanged || maxChanged)
    {
        int oldMin = m_private->min;
        int oldMax = m_private->max;
        m_private->min = min;
        m_private->max = max;
        if (minChanged)
            emit minimumChanged(oldMin);
        if (maxChanged)
            emit maximumChanged(oldMax);
        emit rangeChanged(oldMin, oldMax);
        emit configurationChanged();
    }
}

void SudokuBoard::setBlocks(const QList<QPolygon> &blocks)
{
    QList<QPolygon> newBlocks;
    foreach (QPolygon b, blocks)
    {
        SudokuBoardPrivate::sortBlock(&b);
        if (!newBlocks.contains(b))
            newBlocks.append(b);
    }
    SudokuBoardPrivate::sortBlocks(&newBlocks);

    if (newBlocks != m_private->blocks)
    {
        QList<QPolygon> old = m_private->blocks;
        m_private->blocks = newBlocks;
        m_private->blockMap.clear();

        foreach (QPolygon block, newBlocks)
        {
            foreach (QPoint p, block)
                m_private->blockMap.insert(p, block);
        }

        emit blocksChanged(old);
        emit configurationChanged();
    }
}

SudokuBoardPrivate::SudokuBoardPrivate()
{
    name = QString();
    size = QSize();
    min = -1;
    max = -1;
    blocks = QList<QPolygon>();
    blockMap = QMap<QPoint, QPolygon>();
}

SudokuBoardPrivate::~SudokuBoardPrivate()
{
    blocks.clear();
    blockMap.clear();
}

int SudokuBoardPrivate::loadFromFile(const QString &file, QString *error)
{
    QFile *sdk = new QFile(file);
    if (!sdk->open(QFile::ReadOnly))
    {
        if (error)
            *error = sdk->errorString();
        return -1;
    }

    QString content = QString::fromUtf8(sdk->readAll());
    content = content.replace("\r\n", "\n").replace("\r", "\n");
    QStringList lines = content.split("\n", QString::SkipEmptyParts);

    QString newName;
    QSize newSize;
    int newMin, newMax;
    QList<QPolygon> newBlocks;
    bool foundName = false, foundWidth = false, foundHeight = false, foundMin = false, foundMax = false, foundBlocks = false;

    bool isBlocks = false;
    foreach (QString line, lines)
    {
        line = line.trimmed();

        if (line.startsWith("NAME:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundName)
            {
                if (error)
                    *error = QObject::tr("Duplicate NAME defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                line = line.remove(0, 5);
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                newName = line.trimmed();
                foundName = true;
            }
        }
        else if (line.startsWith("WIDTH:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundWidth)
            {
                if (error)
                    *error = QObject::tr("Duplicate WIDTH defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                line = line.remove(0, 6).trimmed();
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                line = line.remove(" ").remove("\t");
                bool isInt;
                newSize.setWidth(line.toInt(&isInt, 10));
                if (isInt)
                    foundWidth = true;
                else
                {
                    if (error)
                        *error = QObject::tr("Invalid WIDTH.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }
            }
        }
        else if (line.startsWith("HEIGHT:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundHeight)
            {
                if (error)
                    *error = QObject::tr("Duplicate HEIGHT defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                line = line.remove(0, 7).trimmed();
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                line = line.remove(" ").remove("\t");
                bool isInt;
                newSize.setHeight(line.toInt(&isInt, 10));
                if (isInt)
                    foundHeight = true;
                else
                {
                    if (error)
                        *error = QObject::tr("Invalid HEIGHT.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }
            }
        }
        else if (line.startsWith("MINIMUM:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundMin)
            {
                if (error)
                    *error = QObject::tr("Duplicate MINIMUM defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                line = line.remove(0, 8).trimmed();
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                line = line.remove(" ").remove("\t");
                bool isInt;
                newMin = line.toInt(&isInt, 10);
                if (isInt)
                    foundMin = true;
                else
                {
                    if (error)
                        *error = QObject::tr("Invalid MINIMUM.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }
            }
        }
        else if (line.startsWith("MAXIMUM:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundMax)
            {
                if (error)
                    *error = QObject::tr("Duplicate MAXIMUM defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                line = line.remove(0, 8).trimmed();
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                line = line.remove(" ").remove("\t");
                bool isInt;
                newMax = line.toInt(&isInt, 10);
                if (isInt)
                    foundMax = true;
                else
                {
                    if (error)
                        *error = QObject::tr("Invalid MAXIMUM.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }
            }
        }
        else if (line.startsWith("BLOCKS:", Qt::CaseInsensitive))
        {
            if (isBlocks)
            {
                if (error)
                    *error = QObject::tr("Syntax error found.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else if (foundBlocks)
            {
                if (error)
                    *error = QObject::tr("Duplicate BLOCKS defined.");
                sdk->close();
                delete sdk;
                return -1;
            }
            else
            {
                foundBlocks = true;

                line = line.remove(0, 7).trimmed();
                int comment = line.indexOf("#");
                if (comment > -1)
                    line = line.left(comment);
                line = line.remove(" ").remove("\t");

                if (line.endsWith(";"))
                    line.chop(1);
                else
                    isBlocks = true;
                if (line.endsWith(","))
                    line.chop(1);

                if (line.isEmpty())
                    continue;

                if (!line.startsWith("[") || !line.endsWith("]"))
                {
                    if (error)
                        *error = QObject::tr("Syntax error found in BLOCKS.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }

                line = line.remove(0, 1); //Remove [
                line.chop(1); //Remove ]

                QStringList blks = line.split("],[", QString::KeepEmptyParts);
                if (blks.isEmpty() || blks.contains(QString()))
                {
                    if (error)
                        *error = QObject::tr("Empty block found.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }

                foreach (QString blk, blks)
                {
                    if (!blk.startsWith("(") || !blk.endsWith(")"))
                    {
                        if (error)
                            *error = QObject::tr("Syntax error found in BLOCKS.");
                        sdk->close();
                        delete sdk;
                        return -1;
                    }

                    blk = blk.remove(0, 1); //Remove (
                    blk.chop(1); //Remove )

                    QStringList ps = blk.split("),(", QString::KeepEmptyParts);
                    if (ps.isEmpty() || ps.contains(QString()))
                    {
                        if (error)
                            *error = QObject::tr("Empty block found.");
                        sdk->close();
                        delete sdk;
                        return -1;
                    }

                    QPolygon block;

                    foreach (QString pStr, ps)
                    {
                        if (pStr.isEmpty())
                        {
                            if (error)
                                *error = QObject::tr("Invalid block found.");
                            sdk->close();
                            delete sdk;
                            return -1;
                        }

                        QStringList tmp = pStr.split(",", QString::KeepEmptyParts);
                        if (tmp.size() != 2)
                        {
                            if (error)
                                *error = QObject::tr("Invalid block found.");
                            sdk->close();
                            delete sdk;
                            return -1;
                        }

                        bool vx, vy;
                        QPoint p(tmp.first().toInt(&vx, 10), tmp.last().toInt(&vy, 10));
                        if (vx && vy)
                        {
                            if (!block.contains(p))
                                block.append(p);
                        }
                        else
                        {
                            if (error)
                                *error = QObject::tr("Invalid block found.");
                            sdk->close();
                            delete sdk;
                            return -1;
                        }
                    }

                    sortBlock(&block);
                    if (!newBlocks.contains(block))
                        newBlocks.append(block);
                }
            }
        }
        else if (line.startsWith("#"))
            continue;
        else if (isBlocks)
        {
            int comment = line.indexOf("#");
            if (comment > -1)
                line = line.left(comment);
            line = line.remove(" ").remove("\t");

            if (line.endsWith(";"))
            {
                line.chop(1);
                isBlocks = false;
            }
            if (line.endsWith(","))
                line.chop(1);

            if (!line.startsWith("[") || !line.endsWith("]"))
            {
                if (error)
                    *error = QObject::tr("Syntax error found in BLOCKS.");
                sdk->close();
                delete sdk;
                return -1;
            }

            line = line.remove(0, 1); //Remove [
            line.chop(1); //Remove ]

            QStringList blks = line.split("],[", QString::KeepEmptyParts);
            if (blks.isEmpty() || blks.contains(QString()))
            {
                if (error)
                    *error = QObject::tr("Empty block found.");
                sdk->close();
                delete sdk;
                return -1;
            }

            foreach (QString blk, blks)
            {
                if (!blk.startsWith("(") || !blk.endsWith(")"))
                {
                    if (error)
                        *error = QObject::tr("Syntax error found in BLOCKS.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }

                blk = blk.remove(0, 1); //Remove (
                blk.chop(1); //Remove )

                QStringList ps = blk.split("),(", QString::KeepEmptyParts);
                if (ps.isEmpty() || ps.contains(QString()))
                {
                    if (error)
                        *error = QObject::tr("Empty block found.");
                    sdk->close();
                    delete sdk;
                    return -1;
                }

                QPolygon block;

                foreach (QString pStr, ps)
                {
                    if (pStr.isEmpty())
                    {
                        if (error)
                            *error = QObject::tr("Invalid block found.");
                        sdk->close();
                        delete sdk;
                        return -1;
                    }

                    QStringList tmp = pStr.split(",", QString::KeepEmptyParts);
                    if (tmp.size() != 2)
                    {
                        if (error)
                            *error = QObject::tr("Invalid block found.");
                        sdk->close();
                        delete sdk;
                        return -1;
                    }

                    bool vx, vy;
                    QPoint p(tmp.first().toInt(&vx, 10), tmp.last().toInt(&vy, 10));
                    if (vx && vy)
                    {
                        if (!block.contains(p))
                            block.append(p);
                    }
                    else
                    {
                        if (error)
                            *error = QObject::tr("Invalid block found.");
                        sdk->close();
                        delete sdk;
                        return -1;
                    }
                }

                sortBlock(&block);
                if (!newBlocks.contains(block))
                    newBlocks.append(block);
            }
        }
        else
        {
            if (error)
                *error = QObject::tr("The file is invalid.");
            sdk->close();
            delete sdk;
            return -1;
        }
    }

    sdk->close();
    delete sdk;

    if (!foundName)
    {
        if (error)
            *error = QObject::tr("NAME is undefined.");
        return -1;
    }
    if (!foundWidth)
    {
        if (error)
            *error = QObject::tr("WIDTH is undefined.");
        return -1;
    }
    if (!foundHeight)
    {
        if (error)
            *error = QObject::tr("HEIGHT is undefined.");
        return -1;
    }
    if (!foundMin)
    {
        if (error)
            *error = QObject::tr("MINIMUM is undefined.");
        return -1;
    }
    if (!foundMax)
    {
        if (error)
            *error = QObject::tr("MAXIMUM is undefined.");
        return -1;
    }
    if (foundBlocks)
        sortBlocks(&newBlocks);
    else
    {
        if (error)
            *error = QObject::tr("BLOCKS is undefined.");
        return -1;
    }

    if (newName.compare(name) == 0 &&
            newSize == size &&
            newMin == min &&
            newMax == max &&
            newBlocks == blocks)
    {
        if (error)
            error->clear();
        return 1;
    }
    else
    {
        name = newName;
        size = newSize;
        min = newMin;
        max = newMax;
        blocks = newBlocks;

        foreach (QPolygon block, blocks)
        {
            foreach (QPoint p, block)
                blockMap.insert(p, block);
        }

        if (error)
            error->clear();
        return 0;
    }
}

void SudokuBoardPrivate::sortBlock(QPolygon *block)
{
    QMap<QPoint, bool> sort;
    foreach (QPoint p, *block)
        sort.insert(p, true);
    block->clear();
    foreach (QPoint p, sort.keys())
        block->append(p);
}

void SudokuBoardPrivate::sortBlocks(QList<QPolygon> *blocks)
{
    QMap<QPolygon, bool> sort;
    foreach (QPolygon p, *blocks)
        sort.insert(p, true);
    blocks->clear();
    foreach (QPolygon p, sort.keys())
        blocks->append(p);
}
