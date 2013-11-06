#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QTabWidget>

#include <QDebug>
#include <QFile>

#include "studenttablewidget.h"
#include "teachertablewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget(this))
{
//    ui->setupUi(this);
    setWindowTitle(tr("人事管理系统"));
    initUI();

    Student student("Class 1", "Yang", "001", "123456", "male", QDate(2011, 2, 28));
    Teacher teacher("Yang", "001", "123456", "male", QDate(1582, 9, 15), "CS", "vice Prof.");

    QFile file("binary_file");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
        qDebug() << "error open file: " << file.errorString();
    } else {
        QDataStream out(&file);
        out << student;
        qDebug() << student.toString();
        student.setName("Name2");
        out << student;
        qDebug() << student.toString();
        out << teacher;
        qDebug() << teacher.toString();
    }
    file.close();

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << file.errorString();
    } else {
        QDataStream in(&file);
        Student stu2;
        in >> stu2;
        qDebug() << stu2.toString();
        in >> stu2;
        qDebug() << stu2.toString();

        Teacher teach2;
        in >> teach2;
        qDebug() << teach2.toString();

        file.close();
    }
}

void MainWindow::initUI()
{
    tabs->addTab(new StudentTableWidget(), tr("&Student"));
    tabs->addTab(new TeacherTableWidget(), tr("&Teacher"));
    setCentralWidget(tabs);
}

MainWindow::~MainWindow()
{
    delete ui;
}
