#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QElapsedTimer>
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
#include <QTimer>
#include <QUrl>
#include <qmath.h>
#include "aboutdialog.h"
#include "gridboard.h"
#include "sudokuboard.h"
#include "sudokusolver.h"
#include "mainwindow_p.h"
#include "mainwindow.h"

using namespace CIS5603;

static MainWindow *instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    instance = this;

    //This part of code can generate a board quickly.
    /*
    SudokuBoard *bb = new SudokuBoard(this);
    bb->setName("Giant Sudoku");
    bb->setSize(25, 25);
    bb->setRange(1, 25);
    QList<QPolygon> blks;
    QList<QColor> colors;
    for (int i=0; i<5; i++)
    {
        for (int j=0;j<5;j++)
        {
            QPolygon polygon;
            for (int m=0; m<5; m++)
            {
                for (int n=0; n<5; n++)
                    polygon.append(QPoint(m + 5 * i, n + 5 * j));
            }
            blks.append(polygon);
            colors.append((i + j) % 2 == 0 ? QColor("#E5E4E2") : QColor("#FFFFFF"));
        }
    }
    bb->setBlocks(blks);
    bb->setColors(colors);
    bb->saveToFile("Giant.board");*/

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
    m_private->boxBoard->setItemData(m_private->boxBoard->count() - 1, tr("Select..."), Qt::ToolTipRole);
    foreach (SudokuBoard *board, m_private->boards)
    {
        QString text = tr("%1 (%2x%3)").arg(board->name()).arg(QString::number(board->rows(), 10)).arg(QString::number(board->columns(), 10));
        m_private->boxBoard->addItem(text);
        m_private->boxBoard->setItemData(m_private->boxBoard->count() - 1, text, Qt::ToolTipRole);
    }
    m_private->boxBoard->setCurrentIndex(0);
    connect(m_private->boxBoard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onBoardSelected(int)));

    m_private->labelAlgorithm = new QLabel(tr("Algorithm:"), this);

    m_private->boxAlgorithm = new QComboBox(this);
    m_private->boxAlgorithm->addItem(tr("Select..."));
    m_private->boxAlgorithm->setItemData(m_private->boxAlgorithm->count() - 1, tr("Select..."), Qt::ToolTipRole);
    m_private->boxAlgorithm->addItem(tr("Heuristic Search"));
    m_private->boxAlgorithm->setItemData(m_private->boxAlgorithm->count() - 1, tr("Heuristic Search"), Qt::ToolTipRole);
    connect(m_private->boxAlgorithm, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onAlgorithmSelected(int)));

    m_private->checkMultiThread = new QCheckBox(tr("&Multi-threading"), this);
    m_private->checkMultiThread->setChecked(false);

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

    m_private->buttonSolve = new QPushButton(tr("&Solve"), this);
    m_private->buttonSolve->setAutoDefault(false);
    m_private->buttonSolve->setDefault(true);
    connect(m_private->buttonSolve, SIGNAL(clicked(bool)),
            this, SLOT(solveSudoku()));

    int w = qMax(m_private->buttonInitialize->sizeHint().width(), qMax(m_private->buttonPreset->sizeHint().width(), m_private->buttonSolve->sizeHint().width()));
    m_private->buttonInitialize->setFixedWidth(w);
    m_private->buttonPreset->setFixedWidth(w);
    m_private->buttonSolve->setFixedWidth(w);

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

    m_private->labelTitleTime = new QLabel(tr("Total Time:"), this);

    m_private->labelTime = new QLabel("0.000", this);

    m_private->mainWidget = new QWidget(this);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(m_private->labelBoard);
    rightLayout->addWidget(m_private->boxBoard);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(m_private->labelAlgorithm);
    rightLayout->addWidget(m_private->boxAlgorithm);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(m_private->checkMultiThread);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(m_private->buttonPreset, 0, Qt::AlignCenter);
    rightLayout->addWidget(m_private->buttonInitialize, 0, Qt::AlignCenter);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(m_private->buttonSolve, 0, Qt::AlignCenter);
    rightLayout->addSpacing(50);
    rightLayout->addStretch();
    rightLayout->addWidget(m_private->buttonAbout, 0, Qt::AlignCenter);

    QHBoxLayout *labels = new QHBoxLayout();
    labels->setContentsMargins(0, 0, 0, 0);
    labels->addStretch();
    labels->addWidget(m_private->labelTitleTime);
    labels->addWidget(m_private->labelTime);
    labels->addStretch();

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

    QHBoxLayout *layout = new QHBoxLayout(m_private->mainWidget);
    layout->addLayout(leftLayout, 1);
    layout->addLayout(rightLayout, 0);
    m_private->mainWidget->setLayout(layout);

    setCentralWidget(m_private->mainWidget);
    setAcceptDrops(true);

    setWindowTitle(QApplication::applicationName());

    m_private->buttonInitialize->setEnabled(false);
    m_private->buttonPreset->setEnabled(false);
    m_private->updateWidgets();

    m_private->timer = new QTimer(this);
    m_private->timer->setInterval(51);
    connect(m_private->timer, SIGNAL(timeout()),
            this, SLOT(updateTime()));

    m_private->elapsedTimer = new QElapsedTimer();
}

MainWindow::~MainWindow()
{
    instance = 0;
    delete m_private;
}

MainWindow *MainWindow::sharedInstance()
{
    return instance;
}

MainWindow *sharedInstance()
{
    return instance;
}

SudokuBoard *MainWindow::currentBoard()
{
    return m_private->currentBoard;
}

GridBoard *MainWindow::gridBoard()
{
    return m_private->boardWidget;
}

bool MainWindow::multithreadingEnabled() const
{
    return m_private->checkMultiThread->isChecked();
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
                    m_private->boardWidget->setBackgroundColor(i, j, board->color(i, j));
            }
        }

        if (m_private->solver)
            delete m_private->solver;

        m_private->solver = new SudokuSolver(m_private->currentBoard, this);
        connect(m_private->solver, SIGNAL(terminated(QString)),
                this, SLOT(onTerminated(QString)));
        connect(m_private->solver, SIGNAL(finished()),
                this, SLOT(onFinished()));

        onAlgorithmSelected(m_private->boxAlgorithm->currentIndex());

        m_private->buttonInitialize->setEnabled(true);
        m_private->buttonPreset->setEnabled(true);

        adjustBoard();
    }
    else
    {
        if (m_private->solver)
        {
            delete m_private->solver;
            m_private->solver = 0;
        }

        onAlgorithmSelected(m_private->boxAlgorithm->currentIndex());

        m_private->buttonInitialize->setEnabled(false);
        m_private->buttonPreset->setEnabled(false);
        m_private->boardWidget->clear();
        m_private->boardWidget->setSize(0, 0);
    }
    initialize();

    m_private->updateWidgets();
}

void MainWindow::onAlgorithmSelected(int index)
{
    if (m_private->solver)
    {
        if (index < 1)
            m_private->solver->setAlgorithm(QString());
        else
            m_private->solver->setAlgorithm(m_private->boxAlgorithm->itemText(index));
    }

    m_private->updateWidgets();
}

void MainWindow::showAbout()
{
    AboutDialog *dlg = new AboutDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->exec();
}

void MainWindow::solveSudoku()
{
    if (m_private->buttonSolve->isEnabled() && m_private->solver)
    {
        if (m_private->solver->state() == SudokuSolver::Running)
        {
            m_private->solver->stop();

            updateTime();
            m_private->elapsedTimer->invalidate();
            m_private->timer->stop();

            m_private->buttonSolve->setText(tr("&Solve"));

            m_private->blockWidgets(false);
        }
        else
        {
            m_private->blockWidgets(true);

            m_private->buttonSolve->setText(tr("&Stop"));
            m_private->labelTime->setText("0.000");

            int emptyCells = 0;
            int maxCells = m_private->currentBoard->rows() * m_private->currentBoard->columns();
            for (int i=0; i<m_private->currentBoard->rows(); i++)
            {
                for (int j=0; j<m_private->currentBoard->columns(); j++)
                {
                    if (m_private->boardWidget->value(i, j) == 0)
                        emptyCells++;
                }
            }

            if (emptyCells == 0)
            {
                QMessageBox::information(this,
                                         windowTitle(),
                                         tr("The game is already solved."));

                m_private->buttonSolve->setText(tr("&Solve"));

                m_private->blockWidgets(false);
                return;
            }

//            if (emptyCells == maxCells && m_private->currentBoard->rows() == m_private->currentBoard->columns())
//            {
//                qreal root = qSqrt(m_private->currentBoard->rows());
//                int n = qRound(root);
//                if (root == (qreal)n)
//                {
//                    //https://en.wikipedia.org/wiki/Sudoku_solving_algorithms#Blank_Sudoku_grids
//                    for (int i=0; i<m_private->currentBoard->rows(); i++)
//                        for (int j=0; j<m_private->currentBoard->rows(); j++)
//                            m_private->boardWidget->setValue(i, j, (i*n + i/n + j) % (n*n) + 1);

//                    m_private->buttonSolve->setText(tr("&Solve"));

//                    m_private->blockWidgets(false);

//                    return;
//                }
//            }

            if (maxCells - emptyCells < m_private->currentBoard->minimumValues())
            {
                QMessageBox::warning(this,
                                     tr("Warning"),
                                     tr("At least %1 values are needed as preset.").arg(QString::number(m_private->currentBoard->minimumValues(), 10)));

                m_private->buttonSolve->setText(tr("&Solve"));

                m_private->blockWidgets(false);

                return;
            }

            if (m_private->boardWidget->hasError())
            {
                QMessageBox::warning(this,
                                     tr("Warning"),
                                     tr("There is an error in the board, please fix the error first."));

                m_private->buttonSolve->setText(tr("&Solve"));

                m_private->blockWidgets(false);

                return;
            }

            m_private->solver->setValues(m_private->boardWidget->gridValues());

            m_private->elapsedTimer->start();
            m_private->timer->start();

            m_private->solver->compute();
        }
    }
}

void MainWindow::onValueChanged(int row, int column, int oldValue, int newValue)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);

    QPoint p1 = QPoint(-1, -1), p2 = QPoint(1, -1);
    for (int i=0; i<m_private->currentBoard->rows(); i++)
    {
        if (!validateRow(i, &p1, &p2))
        {
            m_private->boardWidget->setHighlight(p1, p2);
            return;
        }
    }
    for (int i=0; i<m_private->currentBoard->columns(); i++)
    {
        if (!validateColumn(i, &p1, &p2))
        {
            m_private->boardWidget->setHighlight(p1, p2);
            return;
        }
    }
    foreach (QPolygon block, m_private->currentBoard->blocks())
    {
        if (!validateBlock(block, &p1, &p2))
        {
            m_private->boardWidget->setHighlight(p1, p2);
            return;
        }
    }
    m_private->boardWidget->setHighlight(p1, p2);
}

void MainWindow::onFinished()
{
    updateTime();
    m_private->elapsedTimer->invalidate();
    m_private->timer->stop();

    QList<QPair<QPoint, int> > vs = m_private->solver->computedValues();
    for (int i=0; i<vs.size(); i++)
    {
        QPair<QPoint, int> p = vs.at(i);
        m_private->boardWidget->setValue(p.first, p.second);
    }

    m_private->buttonSolve->setText(tr("&Solve"));

    m_private->blockWidgets(false);
}

void MainWindow::onTerminated(const QString &message)
{
    updateTime();
    m_private->elapsedTimer->invalidate();
    m_private->timer->stop();

    if (!message.isEmpty())
        QMessageBox::information(this,
                                 windowTitle(),
                                 message);

    m_private->buttonSolve->setText(tr("&Solve"));

    m_private->blockWidgets(false);
}

void MainWindow::loadPreset()
{
    QString defaultFile = m_private->lastPreset;
    if (defaultFile.isEmpty() || !QFile::exists(defaultFile))
    {
        QDir current(qApp->applicationDirPath());
#ifdef Q_OS_MAC
        current.cdUp();
        current.cdUp();
        current.cdUp();
#endif
        defaultFile = current.absolutePath();
    }

    QString sdk = QFileDialog::getOpenFileName(this,
                                               tr("Select Sudoku Preset"),
                                               defaultFile,
                                               "*.sdk");

    if (sdk.isEmpty())
        return;

    presetFromFile(sdk, true);
}

void MainWindow::updateTime()
{
    m_private->labelTime->setText(MainWindowPrivate::msToString(m_private->elapsedTimer->elapsed()));
}

void MainWindow::initialize()
{
    if (m_private->currentBoard)
        m_private->boardWidget->clearContents();

    m_private->labelTime->setText("0.000");
}

bool MainWindow::presetFromFile(const QString &file, bool showError)
{
    m_private->lastPreset = file;

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
                if (v > -1)
                    m_private->boardWidget->setValue(i, j, v);
            }
        }

        if (m_private->solver)
            m_private->solver->reset();

        m_private->labelTime->setText("0.000");

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
                    p1->setX(row);
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
                    p1->setX(row);
                    p1->setY(i);
                }
                if (p2)
                {
                    p2->setX(row);
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
                    p1->setX(i);
                    p1->setY(column);
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
                    p1->setY(column);
                }
                if (p2)
                {
                    p2->setX(checked.value(v));
                    p2->setY(column);
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
            int v = m_private->boardWidget->value(p.x(), p.y());
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
                checked.insert(v, p);
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
    checkMultiThread = 0;
    buttonPreset = 0;
    buttonInitialize = 0;
    buttonAbout = 0;
    blankTopLeft = 0;
    blankBottomRight = 0;
    boardWidget = 0;
    labelTitleTime = 0;
    labelTime = 0;
    buttonSolve = 0;

    currentBoard = 0;

    shown = false;

    solver = 0;
}

MainWindowPrivate::~MainWindowPrivate()
{
    if (!boards.isEmpty())
    {
        if (timer->isActive())
            timer->stop();
        if (elapsedTimer->isValid())
            elapsedTimer->invalidate();

        if (solver)
        {
            solver->blockSignals(true);
            solver->stop();
            solver->deleteLater();
        }

        foreach (SudokuBoard *board, boards)
            delete board;
        boards.clear();

        currentBoard = 0;

        delete labelBoard;
        delete boxBoard;
        delete labelAlgorithm;
        delete boxAlgorithm;
        delete checkMultiThread;
        delete buttonPreset;
        delete buttonInitialize;
        delete buttonAbout;
        delete blankTopLeft;
        delete blankBottomRight;
        delete boardWidget;
        delete labelTitleTime;
        delete labelTime;
        delete buttonSolve;
        delete mainWidget;

        delete timer;
        delete elapsedTimer;
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
                qDebug(QObject::tr("Board \"%1\" from %2 already exists.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
                delete board;
            }
            else
            {
                sort.insert(board->name(), board);
                qDebug(QObject::tr("Loaded board \"%1\" from %2.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
            }
        }
        else
        {
            qDebug(QObject::tr("Board \"%1\" from %2 is invalid.").arg(board->name()).arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath())).toUtf8());
            delete board;
        }
    }

    foreach (QString name, sort.keys())
    {
        SudokuBoard *board = sort.value(name);
        boards.append(board);
    }

    return boards.size();
}

void MainWindowPrivate::updateWidgets()
{
    bool enabled = (solver && boxAlgorithm->currentIndex() > 0);

    labelTitleTime->setEnabled(enabled);
    labelTime->setEnabled(enabled);
    buttonSolve->setEnabled(enabled);
}

void MainWindowPrivate::blockWidgets(bool block)
{
    boxBoard->setEnabled(!block);
    boxAlgorithm->setEnabled(!block);
    checkMultiThread->setEnabled(!block);
    buttonPreset->setEnabled(!block);
    buttonInitialize->setEnabled(!block);
    if (block)
        boardWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        boardWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}


QString MainWindowPrivate::msToString(qint64 ms)
{
    if (ms < 1000)
    {
        QString str = QString::number(ms, 10);
        while (str.size() < 3)
            str = QString("0%1").arg(str);
        return str.prepend("0.");
    }
    else if (ms < 60000)
    {
        qint64 m = ms % 1000;
        qint64 s = (ms - m) / 1000;

        QString str = QString::number(m, 10);
        while (str.size() < 3)
            str = QString("0%1").arg(str);
        return QString("%1.%2").arg(QString::number(s, 10)).arg(str);
    }
    else if (ms < 3600000)
    {
        qint64 m = ms % 1000;
        qint64 tmp = (ms - m) / 1000;
        qint64 sec = tmp % 60;
        qint64 min = (tmp - sec) / 60;

        QString strMS = QString::number(m, 10);
        while (strMS.size() < 3)
            strMS = strMS.prepend('0');

        QString strSEC = QString::number(sec, 10);
        if (strSEC.size() == 1)
            strSEC = strSEC.prepend('0');

        return QString("%1:%2.%3").arg(QString::number(min, 10)).arg(strSEC).arg(strMS);
    }
    else
    {
        qint64 m = ms % 1000;
        qint64 tmp = (ms - m) / 1000;
        qint64 sec = tmp % 60;
        qint64 tmp2 = (tmp - sec) / 60;
        qint64 min = tmp2 % 60;
        qint64 h = (tmp2 - min) / 60;

        QString strMS = QString::number(m, 10);
        while (strMS.size() < 3)
            strMS = strMS.prepend('0');

        QString strSEC = QString::number(sec, 10);
        if (strSEC.size() == 1)
            strSEC = strSEC.prepend('0');

        QString strMIN = QString::number(min, 10);
        if (strMIN.size() == 1)
            strMIN = strMIN.prepend('0');

        return QString("%1:%2:%3.%4").arg(QString::number(h, 10)).arg(strMIN).arg(strSEC).arg(strMS);
    }
}
