#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T17:46:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           gridboard.cpp \
           gridmodel.cpp \
           aboutdialog.cpp \
           sudokuboard.cpp

HEADERS  += mainwindow.h \
            mainwindow_p.h \
            aboutdialog.h \
            aboutdialog_p.h \
            gridboard.h \
            gridboard_p.h \
            gridmodel.h \
            gridmodel_p.h \
            sudokuboard.h \
            sudokuboard_p.h \
            version.h

mac {
  VERSION = 1.0
  QMAKE_INFO_PLIST = Resources/Info.plist
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
  mytarget.commands = $(COPY_DIR) "\"$$PWD/Resources/Boards\"" "\"$$OUT_PWD/Sudoku.app/Contents/Resources/\""
  QMAKE_EXTRA_TARGETS += mytarget
  POST_TARGETDEPS += mytarget
}

win32 {
  RC_FILE = Resources/win.rc

  PWD_WIN = $${PWD}
  DESTDIR_WIN = $${OUT_PWD}
  PWD_WIN ~= s,/,\\,g
  DESTDIR_WIN ~= s,/,\\,g

  CONFIG( debug, debug|release ) {
    mytarget.commands = $(COPY_DIR) "\"$${PWD_WIN}\\Resources\\Boards\"" "\"$${DESTDIR_WIN}\\debug\Boards\""
  } else {
    mytarget.commands = $(COPY_DIR) "\"$${PWD_WIN}\\Resources\\Boards\"" "\"$${DESTDIR_WIN}\\release\Boards\""
  }
    QMAKE_EXTRA_TARGETS += mytarget
    POST_TARGETDEPS += mytarget
}

DISTFILES += \
    Resources/Boards/Dodeka.board \
    Resources/Boards/Giant.board \
    Resources/Boards/Jigsaw.board \
    Resources/Boards/Mini.board \
    Resources/Boards/"Number Place Challenger.board" \
    Resources/Boards/Small.board \
    Resources/Boards/Standard.board \
    Resources/Presets/Jigsaw_9x9.sdk \
    Resources/Presets/Simple_9x9.sdk
