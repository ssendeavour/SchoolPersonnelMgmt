#-------------------------------------------------
#
# Project created by QtCreator 2013-11-05T05:11:36
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

QMAKE_CXX = g++-4.8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SchoolPersonnelMgmt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    student.cpp \
    studenttablewidget.cpp \
    teacher.cpp \
    teachertablewidget.cpp \
    postgraduate.cpp \
    teachingassistant.cpp

HEADERS  += mainwindow.h \
    person.h \
    student.h \
    studenttablewidget.h \
    teacher.h \
    teachertablewidget.h \
    postgraduate.h \
    teachingassistant.h

FORMS    += mainwindow.ui

