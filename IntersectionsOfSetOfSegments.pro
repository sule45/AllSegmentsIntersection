#-------------------------------------------------
#
# Project created by QtCreator 2016-12-07T15:27:28
#
#-------------------------------------------------

QT       += core gui \

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IntersectionsOfSetOfSegments
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oblastcrtanja.cpp \
    algoritam.cpp

HEADERS  += mainwindow.h \
    oblastcrtanja.h \
    algoritam.h

FORMS    += mainwindow.ui
