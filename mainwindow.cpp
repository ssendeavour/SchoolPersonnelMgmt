#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QTabWidget>

#include <QDebug>
#include <QFile>

#include "studenttablewidget.h"
#include "teachertablewidget.h"
#include "teachingassistant.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget(this))
{
//    ui->setupUi(this);
    setWindowTitle(tr("人事管理系统"));
    initUI();
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
