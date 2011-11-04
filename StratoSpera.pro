#-------------------------------------------------
#
# Project created by QtCreator 2011-03-27T23:41:05
#
#-------------------------------------------------

QT       += core gui
CONFIG   += mobility
TARGET = StratoSpera
TEMPLATE = app
MOBILITY += location

SOURCES += main.cpp\
        mainwindow.cpp \
    map.cpp \
    mapview.cpp \
    directorymonitor.cpp \
    marker.cpp \
    statuswidget.cpp \
    metadatawidget.cpp

HEADERS  += mainwindow.h \
    map.h \
    mapview.h \
    directorymonitor.h \
    marker.h \
    statuswidget.h \
    metadatawidget.h
