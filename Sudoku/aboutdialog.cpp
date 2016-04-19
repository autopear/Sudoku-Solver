#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "aboutdialog_p.h"
#include "aboutdialog.h"

using namespace CIS5603;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    m_private = new AboutDialogPrivate();

    m_private->label = new QLabel(tr("Sudoku Solver\n\n"
                                     "Version: %1\n\n"
                                     "Author: Merlin Mao\n\n\n"
                                     "A course project of CIS 5603.").arg(QApplication::applicationVersion()),
                                  this);
    m_private->label->setAlignment(Qt::AlignCenter);

    m_private->labelUrl = new QLabel(QString("<a href=\"https://github.com/autopear/Sudoku-Solver\">%1</a>")
                                     .arg(tr("Home Page")));
    m_private->labelUrl->setAlignment(Qt::AlignCenter);
    m_private->labelUrl->setTextFormat(Qt::RichText);
    m_private->labelUrl->setOpenExternalLinks(true);

    m_private->button = new QPushButton(tr("&Close"), this);
    m_private->button->setDefault(true);
    connect(m_private->button, SIGNAL(clicked(bool)),
            this, SLOT(accept()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_private->label);
    layout->addSpacing(10);
    layout->addWidget(m_private->labelUrl);
    layout->addSpacing(10);
    layout->addWidget(m_private->button, 0, Qt::AlignCenter);
    setLayout(layout);

    setWindowTitle(tr("About %1").arg(QApplication::applicationName()));
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog()
{
    delete m_private;
}

AboutDialogPrivate::~AboutDialogPrivate()
{
    delete label;
    delete labelUrl;
    delete button;
}
