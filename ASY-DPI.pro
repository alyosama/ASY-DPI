TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp




INCLUDEPATH+= $$PWD/../Libraries/PcapPlusPlus/Dist/header
INCLUDEPATH+= /usr/include/netinet

LIBS+= -static-libstdc++
LIBS+=-L$$PWD/../Libraries/PcapPlusPlus/Dist  -lPcap++ -lPacket++ -lCommon++
LIBS+= -lpcap -lpthread


DESTDIR= build

include(deployment.pri)
qtcAddDeployment()

