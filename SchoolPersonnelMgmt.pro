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
    teacher.cpp \
    postgraduate.cpp \
    teachingassistant.cpp \
    studenttablewidget.cpp \
    studenttablemodel.cpp \
    StudentTableDelegate.cpp

HEADERS  += mainwindow.h \
    person.h \
    student.h \
    teacher.h \
    teachertablewidget.h \
    postgraduate.h \
    teachingassistant.h \
    const.h \
    studenttablewidget.h \
    studenttablemodel.h \
    StudentTableDelegate.h

FORMS    += mainwindow.ui

