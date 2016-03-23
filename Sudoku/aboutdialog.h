#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace CIS5603
{

class AboutDialogPrivate;

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    AboutDialogPrivate *m_private;
};

}

#endif // ABOUTDIALOG_H
