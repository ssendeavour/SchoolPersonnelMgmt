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

#include <QSortFilterProxyModel>

#include "studenttablewidget.h"
#include "StudentTableDelegate.h"
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
    // student tab
    QTableView *studentView = new QTableView();
//    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel();
//    sortModel->setSourceModel(new StudentTableModel(studentView));
//    studentView->setModel(sortModel);
//    studentView->setSortingEnabled(true);
    studentView->setModel(new StudentTableModel(studentView));
    studentView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    studentView->horizontalHeader()->setHighlightSections(true);
    studentView->setItemDelegate(new StudentTableDelegate(studentView));
    studentView->resizeColumnsToContents();
    studentView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    studentView->setSelectionBehavior(QAbstractItemView::SelectItems);
    tabs->addTab(studentView, tr("S&tudent"));

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

    fileMenu->addSeparator();

    QAction *quitAction =new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, &qApp->quit);
    fileMenu->addAction(quitAction);
    toolbar->addAction(quitAction);
}

bool MainWindow::openFile()
{
    bool succeed = true;
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open file"), ".",
                tr("Student Data File(*.%1);;All files(*)").arg(CONST::FILE_EXTENSION_STUDENT));
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
    QString error = "";
    QString displayedError = "";
    switch (tabs->currentIndex()) {
    case 0: {  // student table
        QTableView *studentView = qobject_cast<QTableView*>(currentWidget);
        if(studentView){
            QList<Student> list = Student::readFromFile(file, error);
            if(error.length() > 0){
                displayedError = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(fileName).arg(error);
                succeed = false;
                break;
            } else {
                StudentTableModel *model = qobject_cast<StudentTableModel*>(studentView->model());
                if(model){
                    model->setStudentList(list);
                    studentView->resizeColumnsToContents();
                } else {
                    displayedError = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   Unknown").arg(fileName);
                    succeed = false;
                    break;
                }
            }
        } else {
            succeed = false;
            displayedError = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   unknown").arg(fileName);
            break;
        }
        break;
    }
    default:
        break;
    }
    file.close();
    if(!succeed){
        QMessageBox::warning(this, tr("Error"), displayedError);
    }
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

    QString error = "";
    QString displayedError = "";
    QWidget *currentWidget = tabs->currentWidget();
    switch (tabs->currentIndex()) {
    case 0: {  // student table
        QTableView *studentView = qobject_cast<QTableView*>(currentWidget);
        if(studentView){
            StudentTableModel *model = qobject_cast<StudentTableModel *>(studentView->model());
            if(model){
                if(!Student::writeToFile(file, model->getStudentList())){
                    succeed = false;
                    displayedError =tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
                    break;
                }
            } else {
            succeed = false;
            displayedError =tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
            break;
            }
        } else {
            succeed = false;
            displayedError =tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
            break;
        }
        break;
    }
    default:
        break;
    }
    file.close();
    if(!succeed){
        QMessageBox::warning(this, tr("Error"), displayedError);
    }
    return succeed;
}

bool MainWindow::clearAllData()
{
    return true;
}
