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
           aboutdialog.cpp \
           abstractsolver.cpp \
           heuristicsearchsolver.cpp \
           gridboard.cpp \
           gridmodel.cpp \
           sudokuboard.cpp \
           sudokusolver.cpp

HEADERS  += mainwindow.h \
            mainwindow_p.h \
            aboutdialog.h \
            aboutdialog_p.h \
            abstractsolver.h \
            abstractsolver_p.h \
            heuristicsearchsolver.h \
            gridboard.h \
            gridboard_p.h \
            gridmodel.h \
            gridmodel_p.h \
            sudokuboard.h \
            sudokuboard_p.h \
            sudokusolver.h \
            sudokusolver_p.h \
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

linux {
  VERSION = 1.0
  mytarget.commands = $(COPY_DIR) "\"$$PWD/Resources/Boards\"" "\"$$OUT_PWD/Boards\""
  QMAKE_EXTRA_TARGETS += mytarget
  POST_TARGETDEPS += mytarget
}

DISTFILES += \
    Resources/Boards/Dodeka.board \
    Resources/Boards/Giant.board \
    Resources/Boards/Hyper.board \
    Resources/Boards/Jigsaw.board \
    Resources/Boards/Mini.board \
    Resources/Boards/"Number Place Challenger.board" \
    Resources/Boards/Small.board \
    Resources/Boards/Standard.board \
    Resources/Presets/Dodeka/Dodeka_12x12.sdk \
    Resources/Presets/Giant/Giant_25x25.sdk \
    Resources/Presets/Hyper/Hyper_9x9.sdk \
    Resources/Presets/Jigsaw/Jigsaw_9x9.sdk \
    Resources/Presets/Mini/Mini_6x6.sdk \
    Resources/Presets/NPC/NPC_16x16.sdk \
    Resources/Presets/Small/Small_4x4.sdk \
    Resources/Presets/Standard/Easy/Standard_001_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_002_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_003_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_004_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_005_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_006_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_007_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_008_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_009_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_010_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_011_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_012_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_013_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_014_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_015_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_016_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_017_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_018_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_019_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_020_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_021_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_022_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_023_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_024_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_025_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_026_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_027_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_028_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_029_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_030_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_031_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_032_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_033_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_034_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_035_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_036_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_037_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_038_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_039_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_040_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_041_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_042_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_043_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_044_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_045_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_046_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_047_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_048_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_049_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_050_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_051_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_052_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_053_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_054_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_055_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_056_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_057_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_058_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_059_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_060_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_061_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_062_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_063_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_064_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_065_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_066_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_067_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_068_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_069_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_070_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_071_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_072_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_073_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_074_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_075_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_076_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_077_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_078_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_079_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_080_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_081_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_082_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_083_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_084_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_085_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_086_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_087_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_088_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_089_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_090_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_091_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_092_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_093_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_094_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_095_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_096_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_097_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_098_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_099_9x9.sdk \
    Resources/Presets/Standard/Easy/Standard_100_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_001_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_002_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_003_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_004_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_005_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_006_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_007_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_008_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_009_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_010_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_011_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_012_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_013_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_014_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_015_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_016_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_017_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_018_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_019_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_020_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_021_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_022_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_023_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_024_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_025_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_026_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_027_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_028_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_029_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_030_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_031_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_032_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_033_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_034_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_035_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_036_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_037_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_038_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_039_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_040_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_041_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_042_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_043_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_044_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_045_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_046_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_047_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_048_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_049_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_050_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_051_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_052_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_053_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_054_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_055_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_056_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_057_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_058_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_059_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_060_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_061_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_062_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_063_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_064_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_065_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_066_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_067_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_068_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_069_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_070_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_071_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_072_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_073_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_074_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_075_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_076_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_077_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_078_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_079_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_080_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_081_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_082_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_083_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_084_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_085_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_086_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_087_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_088_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_089_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_090_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_091_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_092_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_093_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_094_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_095_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_096_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_097_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_098_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_099_9x9.sdk \
    Resources/Presets/Standard/Hard/Standard_100_9x9.sdk
