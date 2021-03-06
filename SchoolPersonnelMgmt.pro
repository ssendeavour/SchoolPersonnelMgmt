#-------------------------------------------------
#
# Project created by QtCreator 2013-11-05T05:11:36
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

linux-*: QMAKE_CXX = g++-4.8

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
    StudentTableModel.cpp \
    const.cpp \
    CommonTableDelegate.cpp \
    CommonFilterDialog.cpp \
    TeacherTableModel.cpp \
    PostgraduateTableModel.cpp \
    TeachingAssistantTableModel.cpp

HEADERS  += mainwindow.h \
    person.h \
    student.h \
    teacher.h \
    postgraduate.h \
    teachingassistant.h \
    const.h \
    StudentTableModel.h \
    CommonTableDelegate.h \
    CommonFilterDialog.h \
    TeacherTableModel.h \
    PostgraduateTableModel.h \
    TeachingAssistantTableModel.h

FORMS    += mainwindow.ui

RESOURCES += \
    toolbar_icon.qrc

