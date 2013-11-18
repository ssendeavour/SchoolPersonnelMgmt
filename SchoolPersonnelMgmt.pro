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

# ensure every string are wrapped with QObject::tr()(translate) or QLatin1String()(not translate)
#DEFINES += QT_NO_CAST_FROM_ASCII

SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    student.cpp \
    teacher.cpp \
    postgraduate.cpp \
    teachingassistant.cpp \
    StudentTableDelegate.cpp \
    StudentTableModel.cpp \
    StudentFilterDialog.cpp

HEADERS  += mainwindow.h \
    person.h \
    student.h \
    teacher.h \
    postgraduate.h \
    teachingassistant.h \
    const.h \
    StudentTableDelegate.h \
    StudentTableModel.h \
    StudentFilterDialog.h

FORMS    += mainwindow.ui

