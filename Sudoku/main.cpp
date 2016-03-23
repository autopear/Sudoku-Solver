#include <QApplication>
#include "mainwindow.h"
#include "version.h"

using namespace CIS5603;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr(VER_PRODUCTNAME_STR));
    a.setApplicationVersion(VER_PRODUCTVERSION_STR);
    MainWindow w;
    w.show();

    return a.exec();
}
