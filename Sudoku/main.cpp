#include <QApplication>
#include <QDir>
#include <QFile>
#include "mainwindow.h"
#include "version.h"

using namespace CIS5603;

static QString logFile = QString();

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (logFile.isEmpty())
        return;

    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug: %1 (%2:%3, %4)\n")
                .arg(msg)
                .arg(context.file)
                .arg(context.line)
                .arg(context.function);
        break;
    case QtInfoMsg:
        text = QString("Info: %1 (%2:%3, %4)\n")
                .arg(msg)
                .arg(context.file)
                .arg(context.line)
                .arg(context.function);
        break;
    case QtWarningMsg:
        text = QString("Warning: %1 (%2:%3, %4)\n")
                .arg(msg)
                .arg(context.file)
                .arg(context.line)
                .arg(context.function);
        break;
    case QtCriticalMsg:
        text = QString("Critical: %1 (%2:%3, %4)\n")
                .arg(msg)
                .arg(context.file)
                .arg(context.line)
                .arg(context.function);
        break;
    case QtFatalMsg:
        text = QString("Fatal: %1 (%2:%3, %4)\n")
                .arg(msg)
                .arg(context.file)
                .arg(context.line)
                .arg(context.function);
        break;
    }

    fprintf(stderr, "%s", text.toLocal8Bit().constData());

    QFile log(logFile);
    log.open(QFile::WriteOnly | QFile::Append | QFile::Text);
    log.write(text.toUtf8());
    log.close();
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr(VER_PRODUCTNAME_STR));
    a.setApplicationVersion(VER_PRODUCTVERSION_STR);

    QDir current(qApp->applicationDirPath());
#ifdef Q_OS_MAC
    current.cdUp();
    current.cdUp();
    current.cdUp();
#endif
    logFile = current.absoluteFilePath("Sudoku.log");
    if (QFile::exists(logFile))
        QFile::remove(logFile);

    MainWindow w;
    w.show();

    return a.exec();
}
