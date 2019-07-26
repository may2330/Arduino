#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T15:14:40
#
#-------------------------------------------------

QT       += core gui

# This is my code
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LidarPlotExample
TEMPLATE = app
LIBS += -li2c

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    VL53L0X.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    VL53L0X.h

FORMS    += mainwindow.ui


