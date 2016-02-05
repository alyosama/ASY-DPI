TEMPLATE = app
TARGET = ASY-DPI
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    stringmatch.cpp


INCLUDEPATH+= $$PWD/../Libraries/PcapPlusPlus/Dist/header
INCLUDEPATH+= /usr/include/netinet

LIBS+= -static-libstdc++
LIBS+=-L$$PWD/../Libraries/PcapPlusPlus/Dist  -lPcap++ -lPacket++ -lCommon++
LIBS+= -lpcap -lpthread

CONFIG += c++11

DESTDIR= build

include(deployment.pri)
qtcAddDeployment()

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

HEADERS += \
    mainwindow.h \
    packetcapture.h \
    settingsdialog.h \
    common.h \
    stringmatch.h

