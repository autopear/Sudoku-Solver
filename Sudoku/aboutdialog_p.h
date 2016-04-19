#ifndef ABOUTDIALOG_P_H
#define ABOUTDIALOG_P_H

#include "aboutdialog.h"

class QLabel;
class QPushButton;

namespace CIS5603
{

class AboutDialogPrivate
{
    friend class AboutDialog;
protected:
    AboutDialogPrivate() { }
    ~AboutDialogPrivate();

    QLabel *label;
    QLabel *labelUrl;
    QPushButton *button;
};

}

#endif // ABOUTDIALOG_P_H
