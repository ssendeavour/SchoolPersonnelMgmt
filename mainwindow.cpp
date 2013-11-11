#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QTabWidget>

#include <QDebug>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QToolBar>

#include <QFileDialog>
#include <QMessageBox>

#include "studenttablewidget.h"
#include "teachertablewidget.h"
#include "teachingassistant.h"

#include "const.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget(this))
{
//    ui->setupUi(this);
    setWindowTitle(tr("人事管理系统"));
    initUI();
    addMenuBarToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    tabs->addTab(new StudentTableWidget(), tr("&Student"));
    tabs->addTab(new TeacherTableWidget(), tr("&Teacher"));
    setCentralWidget(tabs);
}

void MainWindow::addMenuBarToolBar()
{
    QToolBar *toolbar = addToolBar(tr("&File"));
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    QAction *openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);
    toolbar->addAction(openAction);

    QAction *saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);
    toolbar->addAction(saveAction);

    fileMenu->addSeparator();

    QAction *clearDataAction = new QAction(tr("&C&lear data"), this);
    connect(clearDataAction, &QAction::triggered, this, &MainWindow::clearAllData);
    fileMenu->addAction(clearDataAction);
    toolbar->addAction(clearDataAction);
}

bool MainWindow::openFile()
{
    bool succeed = true;
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open file"), ".",
                tr("Personnel Student Data File(*.%1);;All files(*)").arg(CONST::FILE_EXTENSION_STUDENT));
    if(fileName.isEmpty()){
        return false;
    }
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, tr("Open File"),
                             tr("Can't open file: \n\n%1\n\n%2").arg(fileName).arg(file.errorString()));
        return false;
    }

    QWidget *currentWidget = tabs->currentWidget();
    switch (tabs->currentIndex()) {
    case 0: {  // student table
        StudentTableWidget *studentWidget = qobject_cast<StudentTableWidget*>(currentWidget);
        if(studentWidget){
            if(!studentWidget->openFromFile(file)){
                QMessageBox::warning(this, tr("Read file error"),
                                     tr("Read file\n\n%1\n\n failed, reason: %2").arg(fileName).arg(file.errorString()));
                succeed = false;
            }
        } else {
            succeed = false;
        }
        break;
    }
    default:
        break;
    }
    file.close();
    return succeed;
}

bool MainWindow::saveFile()
{
    bool succeed = true;
    bool addedExtension = false;
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Save"), ".",
                tr("Personnel Student Data File(*.%1);;All files(*)").arg(CONST::FILE_EXTENSION_STUDENT));
    if(fileName.isEmpty()){
        return false;
    }
    // add extension if necessary
    if(!fileName.endsWith("." + CONST::FILE_EXTENSION_STUDENT)){
        fileName +=  "." + CONST::FILE_EXTENSION_STUDENT;
    }

    QFile file(fileName);
    if(addedExtension && file.exists()){
        if(QMessageBox::Yes != QMessageBox::question(this, tr("File exists"),
                              tr("The file\n\n%1\n\nalready exist, replace it ?")
                              .arg(fileName))){
            return false;
        }
    }

    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, tr("Save File"),
                             tr("Can't open file: \n\n%1\n\n%2").arg(fileName).arg(file.errorString()));
        return false;
    }

    QWidget *currentWidget = tabs->currentWidget();
    switch (tabs->currentIndex()) {
    case 0: {  // student table
        StudentTableWidget *studentWidget = qobject_cast<StudentTableWidget*>(currentWidget);
        if(studentWidget){
            if(!studentWidget->saveDataToFile(file)){
                QMessageBox::warning(this, tr("Save file error"),
                                     tr("Save file\n\n%1\n\n failed, reason: %2").arg(fileName).arg(file.errorString()));
                succeed = false;
            }
        } else {
            succeed = false;
        }
        break;
    }
    default:
        break;
    }
    file.close();
    return succeed;
}

bool MainWindow::clearAllData()
{
}
