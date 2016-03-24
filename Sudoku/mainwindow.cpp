#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QResizeEvent>
#include <QUrl>
#include "aboutdialog.h"
#include "gridboard.h"
#include "sudokuboard.h"
#include "mainwindow_p.h"
#include "mainwindow.h"

using namespace CIS5603;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_private = new MainWindowPrivate();
    m_private->parent = this;

    if (m_private->loadBoards() < 1)
    {
        QMessageBox::critical(0,
                              tr("Error"),
                              tr("Cannot load boards for this game."));
        exit(-1);
    }

    m_private->labelBoard = new QLabel(tr("Board:"), this);

    m_private->boxBoard = new QComboBox(this);
    m_private->boxBoard->addItem(tr("Select..."));
    foreach (SudokuBoard *board, m_private->boards)
        m_private->boxBoard->addItem(QObject::tr("%1 (%2x%3)").arg(board->name()).arg(QString::number(board->rows(), 10)).arg(QString::number(board->columns(), 10)));
    m_private->boxBoard->setCurrentIndex(0);
    connect(m_private->boxBoard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onBoardSelected(int)));

    m_private->labelAlgorithm = new QLabel(tr("Algorithm:"), this);

    m_private->boxAlgorithm = new QComboBox(this);
    m_private->boxAlgorithm->addItem(tr("Select..."));
    connect(m_private->boxAlgorithm, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onAlgorithmSelected(int)));

    m_private->buttonPreset = new QPushButton(tr("&Preset"), this);
    m_private->buttonPreset->setToolTip(tr("Load a .sdk file with preset values into the board.\n\nYou can also drag a .sdk file to the main window."));
    m_private->buttonPreset->setAutoDefault(false);
    connect(m_private->buttonPreset, SIGNAL(clicked(bool)),
            this, SLOT(loadPreset()));

    m_private->buttonInitialize = new QPushButton(tr("&Initialize"), this);
    m_private->buttonInitialize->setToolTip(tr("Reset all values to empty in the board."));
    m_private->buttonInitialize->setAutoDefault(false);
    connect(m_private->buttonInitialize, SIGNAL(clicked(bool)),
            this, SLOT(initialize()));

    int w = qMax(m_private->buttonInitialize->sizeHint().width(), m_private->buttonPreset->sizeHint().width());
    m_private->buttonInitialize->setFixedWidth(w);
    m_private->buttonPreset->setFixedWidth(w);

    m_private->boxBoard->setMaximumWidth(w * 1.5);
    m_private->boxAlgorithm->setMaximumWidth(w * 1.5);

    m_private->buttonAbout = new QPushButton(tr("&About"), this);
    m_private->buttonAbout->setAutoDefault(false);
    connect(m_private->buttonAbout, SIGNAL(clicked(bool)),
            this, SLOT(showAbout()));

    m_private->blankTopLeft = new QWidget(this);

    m_private->blankBottomRight = new QWidget(this);

    m_private->boardWidget = new GridBoard(this);
    connect(m_private->boardWidget, SIGNAL(valueChanged(int,int,int,int)),
            this, SLOT(onValueChanged(int,int,int,int)));

    m_private->labelTitleTotalTime = new QLabel(tr("Total Time:"), this);

    m_private->labelTotalTime = new QLabel(tr("0 ms"), this);

    m_private->labelTitleStepTime = new QLabel(tr("Current Step:"), this);

    m_private->labelStepTime = new QLabel(tr("0 ms"), this);

    m_private->buttonNext = new QPushButton(tr("&Next"), this);
    m_private->buttonNext->setAutoDefault(false);
    m_private->buttonNext->setDefault(true);
    connect(m_private->buttonNext, SIGNAL(clicked(bool)),
            this, SLOT(nextStep()));

    m_private->buttonEnd = new QPushButton(tr("&Go to End"), this);
    m_private->buttonEnd->setAutoDefault(false);
    connect(m_private->buttonEnd, SIGNAL(clicked(bool)),
            this, SLOT(goToEnd()));

    w = qMax(m_private->buttonNext->sizeHint().width(), m_private->buttonEnd->sizeHint().width());
    m_private->buttonNext->setFixedWidth(w);
    m_private->buttonEnd->setFixedWidth(w);

    m_private->mainWidget = new QWidget(this);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(m_private->labelBoard);
    rightLayout->addWidget(m_private->boxBoard);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(m_private->labelAlgorithm);
    rightLayout->addWidget(m_private->boxAlgorithm);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(m_private->buttonPreset, 0, Qt::AlignCenter);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(m_private->buttonInitialize, 0, Qt::AlignCenter);
    rightLayout->addStretch();
    rightLayout->addWidget(m_private->buttonAbout, 0, Qt::AlignCenter);

    QHBoxLayout *labels = new QHBoxLayout();
    labels->setContentsMargins(0, 0, 0, 0);
    labels->addStretch();
    labels->addWidget(m_private->labelTitleTotalTime);
    labels->addWidget(m_private->labelTotalTime);
    labels->addStretch();
    labels->addWidget(m_private->labelTitleStepTime);
    labels->addWidget(m_private->labelStepTime);
    labels->addStretch();

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->setContentsMargins(0, 0, 0, 0);
    buttons->addStretch();
    buttons->addWidget(m_private->buttonNext);
    buttons->addStretch();
    buttons->addWidget(m_private->buttonEnd);
    buttons->addStretch();

    QGridLayout *boardLayout = new QGridLayout();
    boardLayout->setContentsMargins(0, 0, 0, 0);
    boardLayout->setSpacing(0);
    boardLayout->addWidget(m_private->blankTopLeft, 0, 0);
    boardLayout->addWidget(m_private->boardWidget, 1, 1);
    boardLayout->addWidget(m_private->blankBottomRight, 2, 2);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addLayout(boardLayout);
    leftLayout->addLayout(labels);
    leftLayout->addSpacing(10);
    leftLayout->addLayout(buttons);

    QHBoxLayout *layout = new QHBoxLayout(m_private->mainWidget);
    layout->addLayout(leftLayout, 1);
    layout->addLayout(rightLayout, 0);
    m_private->mainWidget->setLayout(layout);

    setCentralWidget(m_private->mainWidget);
    setAcceptDrops(true);

    setWindowTitle(QApplication::applicationName());

    m_private->buttonInitialize->setEnabled(false);
    m_private->buttonPreset->setEnabled(false);
    m_private->buttonNext->setEnabled(false);
    m_private->buttonEnd->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete m_private;
}

SudokuBoard *MainWindow::currentBoard()
{
    return m_private->currentBoard;
}

void MainWindow::onBoardSelected(int index)
{
    if (index < 0)
        return;

    SudokuBoard *board = index == 0 ? 0 : m_private->boards.at(index - 1);
    if (m_private->currentBoard == board)
        return;

    m_private->currentBoard = board;

    m_private->boardWidget->clear();

    if (board)
    {
        m_private->boardWidget->setSize(board->rows(), board->columns());
        if (board->colors().isEmpty())
        {
            for (int i=0; i<board->rows(); i++)
            {
                for (int j=0; j<board->columns(); j++)
                    m_private->boardWidget->setBackgroundColor(i, j, QColor(Qt::white));
            }
        }
        else
        {
            for (int i=0; i<board->rows(); i++)
            {
                for (int j=0; j<board->columns(); j++)
                {
                    QPolygon block = board->findBlock(i, j);
                    if (!block.isEmpty())
                    {
                        int idx = board->blocks().indexOf(block);
                        m_private->boardWidget->setBackgroundColor(i, j, board->colors().at(idx));
                    }
                }
            }
        }

        m_private->buttonInitialize->setEnabled(true);
        m_private->buttonPreset->setEnabled(true);
        m_private->buttonNext->setEnabled(true);
        m_private->buttonEnd->setEnabled(true);

        adjustBoard();
    }
    else
    {
        m_private->buttonInitialize->setEnabled(false);
        m_private->buttonPreset->setEnabled(false);
        m_private->buttonNext->setEnabled(false);
        m_private->buttonEnd->setEnabled(false);
        m_private->boardWidget->clear();
    }
    initialize();
}

void MainWindow::onAlgorithmSelected(int index)
{

}

void MainWindow::showAbout()
{
    AboutDialog *dlg = new AboutDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->exec();
}

void MainWindow::nextStep()
{
    //To do...
}

void MainWindow::goToEnd()
{
    //To do...
}

void MainWindow::stop()
{
    //To do...
}

void MainWindow::onValueChanged(int row, int column, int oldValue, int newValue)
{
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);

    QPoint p1 = QPoint(-1, -1), p2 = QPoint(1, -1);
    if (!validateRow(row, &p1, &p2))
    {
        m_private->boardWidget->setHighlight(p1, p2);
        return;
    }
    if (!validateColumn(column, &p1, &p2))
    {
        m_private->boardWidget->setHighlight(p1, p2);
        return;
    }
    if (!validateBlock(m_private->currentBoard->findBlock(column, row), &p1, &p2))
    {
        m_private->boardWidget->setHighlight(p1, p2);
        return;
    }
    m_private->boardWidget->setHighlight(p1, p2);
}

void MainWindow::loadPreset()
{
#ifdef Q_OS_MAC
    QDir current(qApp->applicationDirPath());
    current.cdUp();
    current.cdUp();

    QString sdk = QFileDialog::getOpenFileName(this,
                                               tr("Select Sudoku Preset"),
                                               current.absolutePath(),
                                               "*.sdk");
#else
    QString sdk = QFileDialog::getOpenFileName(this,
                                               tr("Select Sudoku Preset"),
                                               qApp->applicationDirPath(),
                                               "*.sdk");
#endif

    if (sdk.isEmpty())
        return;

    presetFromFile(sdk, true);
}

void MainWindow::initialize()
{
    if (m_private->currentBoard)
        m_private->boardWidget->clearContents();

    m_private->labelStepTime->setText("0 ms");
    m_private->labelTotalTime->setText("0 ms");
}

bool MainWindow::presetFromFile(const QString &file, bool showError)
{
    if (!m_private->currentBoard)
    {
        if (showError)
            QMessageBox::warning(this,
                                 tr("No Board Selected"),
                                 tr("You must select a board first."));
        return false;
    }

    QFile *sdk = new QFile(file);
    if (!sdk->open(QFile::ReadOnly | QFile::Text))
    {
        if (showError)
            QMessageBox::warning(this,
                                 tr("Error Openning Preset"),
                                 tr("The preset file cannot be opened.\n\n%1")
                                 .arg(sdk->errorString()));
        delete sdk;
        return false;
    }

    QList<QList<int> > presets;
    int lineCount = 0;
    while (!sdk->atEnd())
    {
        lineCount++;

        QString line = QString::fromUtf8(sdk->readLine()).remove("\r").remove("\n").remove(" ").remove("\t");
        int comment = line.indexOf("#");
        if (comment > -1)
            line = line.left(comment);
        if (line.isEmpty())
            continue;

        QStringList list = line.split(",", QString::KeepEmptyParts);
        if (list.size() != m_private->currentBoard->columns())
        {
            if (showError)
                QMessageBox::warning(this,
                                     tr("Invalid Preset"),
                                     tr("Incorrect number of values in row %1.")
                                     .arg(QString::number(lineCount, 10)));

            sdk->close();
            delete sdk;
            return false;
        }
        QList<int> rowValues;
        for (int i=0; i<list.size(); i++)
        {
            QString vStr = list.at(i);
            if (vStr.isEmpty())
                rowValues.append(0);
            else
            {
                bool ok;
                int v = vStr.toInt(&ok, 10);
                if (!ok || v < 0)
                {
                    if (showError)
                        QMessageBox::warning(this,
                                             tr("Invalid Preset"),
                                             tr("The value at row %1 column %2 is not a valid integer.")
                                             .arg(QString::number(lineCount, 10))
                                             .arg(QString::number(i+1, 10)));
                    sdk->close();
                    delete sdk;
                    return false;
                }

                rowValues.append(v);
            }
        }

        presets.append(rowValues);
    }

    if (presets.size() > m_private->currentBoard->rows())
    {
        if (showError)
            QMessageBox::warning(this,
                                 tr("Invalid Preset"),
                                 tr("Too many rows."));
        sdk->close();
        delete sdk;
        return false;
    }
    else if (presets.size() < m_private->currentBoard->rows())
    {
        if (showError)
            QMessageBox::warning(this,
                                 tr("Invalid Preset"),
                                 tr("Too few rows."));
        sdk->close();
        delete sdk;
        return false;
    }
    else
    {
        sdk->close();
        delete sdk;

        m_private->boardWidget->clearContents();

        for (int i=0; i<presets.size(); i++)
        {
            QList<int> rowValues = presets.at(i);
            for (int j=0; j<rowValues.size(); j++)
            {
                int v = rowValues.at(j);
                if (v > 0)
                    m_private->boardWidget->setValue(i, j, v);
            }
        }

        return true;
    }
}

bool MainWindow::setValue(int row, int column, int value)
{
    if (m_private->currentBoard &&
            row > -1 && row < m_private->currentBoard->rows() &&
            column > -1 && column < m_private->currentBoard->columns())
    {
        m_private->boardWidget->setValue(row, column, value);
        return true;
    }

    return false;
}

bool MainWindow::validateRow(int row, QPoint *p1, QPoint *p2)
{
    if (m_private->currentBoard && row > -1 && row < m_private->currentBoard->rows())
    {
        QMap<int, int> checked;
        for (int i=0; i<m_private->currentBoard->columns(); i++)
        {
            int v = m_private->boardWidget->value(row, i);
            if (v == 0)
                continue;
            else if (v < m_private->currentBoard->minimum() || v > m_private->currentBoard->maximum())
            {
                if (p1)
                {
                    p1->setX(i);
                    p1->setY(row);
                }
                if (p2)
                    *p2 = QPoint(-1, -1);
                return false;
            }
            else if (checked.keys().contains(v))
            {
                if (p1)
                {
                    p1->setX(i);
                    p1->setY(row);
                }
                if (p2)
                {
                    p2->setX(checked.value(v));
                    p2->setY(row);
                }
                return false;
            }
            else
                checked.insert(v, i);
        }
        if (p1)
            *p1 = QPoint(-1, -1);
        if (p2)
            *p2 = QPoint(-1, -1);
        return true;
    }
    if (p1)
        *p1 = QPoint(-1, -1);
    if (p2)
        *p2 = QPoint(-1, -1);
    return false;
}

bool MainWindow::validateColumn(int column, QPoint *p1, QPoint *p2)
{
    if (m_private->currentBoard && column > -1 && column < m_private->currentBoard->columns())
    {
        QMap<int, int> checked;
        for (int i=0; i<m_private->currentBoard->rows(); i++)
        {
            int v = m_private->boardWidget->value(i, column);
            if (v == 0)
                continue;
            else if (v < m_private->currentBoard->minimum() || v > m_private->currentBoard->maximum())
            {
                if (p1)
                {
                    p1->setX(column);
                    p1->setY(i);
                }
                if (p2)
                    *p2 = QPoint(-1, -1);
                return false;
            }
            else if (checked.keys().contains(v))
            {
                if (p1)
                {
                    p1->setX(column);
                    p1->setY(i);
                }
                if (p2)
                {
                    p2->setX(column);
                    p2->setY(checked.value(v));
                }
                return false;
            }
            else
                checked.insert(v, i);
        }
        if (p1)
            *p1 = QPoint(-1, -1);
        if (p2)
            *p2 = QPoint(-1, -1);
        return true;
    }
    if (p1)
        *p1 = QPoint(-1, -1);
    if (p2)
        *p2 = QPoint(-1, -1);
    return false;
}

bool MainWindow::validateBlock(const QPolygon &block, QPoint *p1, QPoint *p2)
{
    if (m_private->currentBoard && !block.isEmpty())
    {
        QPolygon uniques;
        foreach (QPoint p, block)
        {
            if (p.x() < 0 || p.x() >= m_private->currentBoard->rows() ||
                    p.x() < 0 || p.x() >= m_private->currentBoard->columns())
            {
                if (p1)
                    *p1 = QPoint(-1, -1);
                if (p2)
                    *p2 = QPoint(-1, -1);
                return false;
            }
            else
                uniques.append(p);
        }

        QMap<int, QPoint> checked;
        foreach (QPoint p, uniques)
        {
            int v = m_private->boardWidget->value(p.y(), p.x());
            if (v == 0)
                continue;
            else if (v < m_private->currentBoard->minimum() || v > m_private->currentBoard->maximum())
            {
                if (p1)
                    *p1 = p;
                if (p2)
                    *p2 = QPoint(-1, -1);
                return false;
            }
            else if (checked.keys().contains(v))
            {
                if (p1)
                    *p1 = p;
                if (p2)
                    *p2 = checked.value(v);
                return false;
            }
            else
            {
                checked.insert(v, p);
            }

        }
        if (p1)
            *p1 = QPoint(-1, -1);
        if (p2)
            *p2 = QPoint(-1, -1);
        return true;
    }
    if (p1)
        *p1 = QPoint(-1, -1);
    if (p2)
        *p2 = QPoint(-1, -1);
    return false;
}

void MainWindow::adjustBoard()
{
    if (!m_private->shown)
        return;

    int mw = m_private->mainWidget->width() - m_private->boardLeft - m_private->boardRight;
    int mh = m_private->mainWidget->height() - m_private->boardTop - m_private->boardBottom;

    int cw = mw;
    int ch = m_private->boardWidget->heightForWidth(mw);
    if (ch > mh)
    {
        cw = mw * mh / ch;
        ch = mh;
    }

    m_private->blankTopLeft->setFixedSize((mw - cw) / 2, (mh - ch) / 2);
    m_private->blankBottomRight->setFixedSize((mw - cw) / 2, (mh - ch) / 2);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (m_private->currentBoard && event && event->mimeData() && event->mimeData()->urls().size() == 1)
    {
        QUrl url = event->mimeData()->urls().first();
        if (url.isLocalFile() && url.toLocalFile().endsWith(".sdk", Qt::CaseInsensitive))
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if (m_private->currentBoard && event && event->mimeData() && event->mimeData()->urls().size() == 1)
    {
        QUrl url = event->mimeData()->urls().first();
        if (url.isLocalFile() && url.toLocalFile().endsWith(".sdk", Qt::CaseInsensitive))
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (m_private->currentBoard && event && event->mimeData() && event->mimeData()->urls().size() == 1)
    {
        QUrl url = event->mimeData()->urls().first();
        if (url.isLocalFile() && url.toLocalFile().endsWith(".sdk", Qt::CaseInsensitive))
        {
            event->acceptProposedAction();
            presetFromFile(url.toLocalFile(), true);
            return;
        }
    }
    event->ignore();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if (!m_private->shown)
    {
        m_private->shown = true;

        QRect out = m_private->mainWidget->geometry();
        QRect in = m_private->boardWidget->geometry();

        m_private->boardTop = in.y();
        m_private->boardLeft = in.x();
        m_private->boardRight = out.right() - in.right();
        m_private->boardBottom = out.bottom() - in.bottom();

        adjustBoard();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    adjustBoard();
}

MainWindowPrivate::MainWindowPrivate()
{
    mainWidget = 0;
    labelBoard = 0;
    boxBoard = 0;
    labelAlgorithm = 0;
    boxAlgorithm = 0;
    buttonPreset = 0;
    buttonInitialize = 0;
    buttonAbout = 0;
    blankTopLeft = 0;
    blankBottomRight = 0;
    boardWidget = 0;
    labelTitleTotalTime = 0;
    labelTotalTime = 0;
    labelTitleStepTime = 0;
    labelStepTime = 0;
    buttonNext = 0;
    buttonEnd = 0;

    currentBoard = 0;

    shown = false;
}

MainWindowPrivate::~MainWindowPrivate()
{
    if (!boards.isEmpty())
    {
        foreach (SudokuBoard *board, boards)
            delete board;
        boards.clear();

        currentBoard = 0;

        delete labelBoard;
        delete boxBoard;
        delete labelAlgorithm;
        delete boxAlgorithm;
        delete buttonPreset;
        delete buttonInitialize;
        delete buttonAbout;
        delete blankTopLeft;
        delete blankBottomRight;
        delete boardWidget;
        delete labelTitleTotalTime;
        delete labelTotalTime;
        delete labelTitleStepTime;
        delete labelStepTime;
        delete buttonNext;
        delete buttonEnd;
        delete mainWidget;
    }
}

int MainWindowPrivate::loadBoards()
{
    QDir current(qApp->applicationDirPath());
#ifdef Q_OS_MAC
    if (!current.cdUp())
        return -1;
    if (!current.cd("Resources"))
    {
        qWarning(QObject::tr("Cannot find Resources directory.").toUtf8());
        return -1;
    }
#endif
    if (!current.cd("Boards"))
    {
        qWarning(QObject::tr("Cannot find Boards directory.").toUtf8());
        return -1;
    }

    QMap<QString, SudokuBoard *> sort;
    foreach (QFileInfo fileInfo, current.entryInfoList(QStringList() << "*.board", QDir::Files))
    {
        SudokuBoard *board = new SudokuBoard(fileInfo.absoluteFilePath(), parent);
        if (board->isValid())
        {
            if (sort.keys().contains(board->name()))
            {
                delete board;
                qDebug(QObject::tr("Board \"%1\" from %2 already exists.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
            }
            else
            {
                sort.insert(board->name(), board);
                qDebug(QObject::tr("Loaded board \"%1\" from %2.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
            }
        }
        else
        {
            delete board;
            qDebug(QObject::tr("Board \"%1\" from %2 is invalid.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
        }
    }

    foreach (QString name, sort.keys())
    {
        SudokuBoard *board = sort.value(name);
        boards.append(board);
    }

    return boards.size();
}
