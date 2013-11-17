#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

#include <QDebug>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QToolBar>

#include <QFileDialog>
#include <QMessageBox>

#include <QModelIndexList>
#include <QItemSelectionModel>

#include "teachingassistant.h"

#include "const.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs_(new QTabWidget(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("Personnel Management System"));
    initUI();
    addMenuBarToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    for(int i=0; i<TAB_NUMBER; ++i){
        this->tableView_[i] = new QTableView(this);
        this->tableView_[i]->setSortingEnabled(true);
        this->tableView_[i]->setSelectionMode(QAbstractItemView::ExtendedSelection);
        this->tableView_[i]->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    initStudentTab();

    this->tabs_->addTab(this->tableView_[MainWindow::TAB::STUDENT], tr("&Student"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::TEACHER], tr("&Teacher"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::POSTGRAD], tr("&Postgraduate"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::TA], tr("Teaching &Assistant"));
    setCentralWidget(this->tabs_);
}

void MainWindow::initStudentTab()
{
    const int index = static_cast<int>(MainWindow::TAB::STUDENT);
    QStringList stuTableHeader;
    QVector<CONST::HDG> indexMap(static_cast<int>(CONST::HDG::COUNT));
    stuTableHeader.append(tr("ID"));
    indexMap[0] = CONST::HDG::ID;
    stuTableHeader.append(tr("Name"));
    indexMap[1] = CONST::HDG::NAME;
    stuTableHeader.append(tr("Sex"));
    indexMap[2] = CONST::HDG::SEX;
    stuTableHeader.append(tr("Birthday"));
    indexMap[3] = CONST::HDG::BIRTHDAY;
    stuTableHeader.append(tr("ID number"));
    indexMap[4] = CONST::HDG::IDNUMBER;
    this->studentTableModel_ = new StudentTableModel(
                indexMap, stuTableHeader, this);
//    this->sortFilterProxyModel_[index]->setSourceModel(this->studentTableModel_);
    this->tableView_[index]->setModel(this->studentTableModel_);
    this->tableView_[index]->setItemDelegate(new StudentTableDelegate(indexMap, this->tableView_[index]));
    this->tableView_[index]->resizeColumnsToContents();
}

void MainWindow::addMenuBarToolBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    fileMenu->addSeparator();

    QAction *quitAction =new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, &qApp->quit);
    fileMenu->addAction(quitAction);

    // Edit menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *addRowAction = new QAction(tr("&Add row"), this);
    addRowAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    connect(addRowAction, &QAction::triggered, this, &MainWindow::addNewRow);
    editMenu->addAction(addRowAction);

    QAction *insertRowAction = new QAction(tr("&Insert row before"), this);
    insertRowAction->setToolTip(tr("Insert a new row before selected row"));
    insertRowAction->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(insertRowAction, &QAction::triggered, this, &MainWindow::insertRowBefore);
    editMenu->addAction(insertRowAction);

    QAction *deleteRowsAction = new QAction(tr("&Delete selected row(s)"), this);
    deleteRowsAction->setShortcut(QKeySequence::Delete);
    connect(deleteRowsAction, &QAction::triggered, this, &MainWindow::deleteRows);
    editMenu->addAction(deleteRowsAction);

    fileMenu->addSeparator();

    QAction *clearDataAction = new QAction(tr("&C&lear data"), this);
    connect(clearDataAction, &QAction::triggered, this, &MainWindow::clearAllData);
    editMenu->addAction(clearDataAction);

    ui->mainToolBar->addAction(openAction);
    ui->mainToolBar->addAction(saveAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(addRowAction);
    ui->mainToolBar->addAction(insertRowAction);
    ui->mainToolBar->addAction(deleteRowsAction);
    ui->mainToolBar->addAction(clearDataAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(quitAction);

    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
}

bool MainWindow::openFile()
{
    int index = this->tabs_->currentIndex();
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

    QString error = "";
    switch (static_cast<MainWindow::TAB>(index)) {
    case MainWindow::TAB::STUDENT: {  // student table
        succeed = openStudentFile(file, error);
        break;
    }
    default:
        break;
    }

    file.close();
    if(succeed){
        this->tableView_[index]->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, tr("Error"), error);
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
    switch (static_cast<MainWindow::TAB>(this->tabs_->currentIndex())) {
    case MainWindow::TAB::STUDENT: {  // student table
        succeed = saveStudentFile(file, error);
    }
    default:
        break;
    }
    file.close();
    if(!succeed){
        QMessageBox::warning(this, tr("Error"), error);
    }
    return succeed;
}

// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openStudentFile(QFile &file, QString &error){
    QList<Student> list = Student::readFromFile(file, error);
    if(error.length() > 0){
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }
    this->studentTableModel_->setDataList(list);
    return true;
}

bool MainWindow::saveStudentFile(QFile &file, QString &error){
    if(!Student::writeToFile(file, this->studentTableModel_->getDataList())){
        error = tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
        return false;
    }
    return true;
}

void MainWindow::clearAllData()
{
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->tabs_->currentIndex());
    switch(tab){
    case MainWindow::TAB::STUDENT:
        this->studentTableModel_->setDataList(QList<Student>());
        break;

    case MainWindow::TAB::TEACHER:
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
}

void MainWindow::addNewRow()
{
    int tabIndex = this->tabs_->currentIndex();
    switch(static_cast<MainWindow::TAB>(tabIndex)){
    case MainWindow::TAB::STUDENT:
        this->studentTableModel_->insertRows(this->studentTableModel_->rowCount(), 1);
        break;

    case MainWindow::TAB::TEACHER:
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
    this->tableView_[tabIndex]->resizeColumnsToContents();
}

void MainWindow::insertRowBefore()
{
    int tabIndex = this->tabs_->currentIndex();
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(tabIndex);
    QItemSelectionModel *selectionModel = this->tableView_[tabIndex]->selectionModel();
    if(!selectionModel->hasSelection()){
        int rowCount = 0;
        switch (tab){
        case MainWindow::TAB::STUDENT:
            rowCount = this->studentTableModel_->rowCount();
            break;

        case MainWindow::TAB::TEACHER:
            break;

        case MainWindow::TAB::POSTGRAD:
            break;

        case MainWindow::TAB::TA:
            break;
        }

        if(rowCount > 0){
            QMessageBox::warning(this, tr("No row selected"),
                     tr("Please select a row before which new row will be inserted"));
        } else {
            addNewRow();
        }
        return;
    }
    if(selectionModel->selectedRows().size() != 1){
        QMessageBox::warning(this, tr("Multiple rows selected"), tr("Please select only one row."));
        return;
    }
    switch(tab){
    case MainWindow::TAB::STUDENT:
        this->studentTableModel_->insertRows(selectionModel->selectedRows().at(0).row(), 1);
        this->tableView_[tabIndex]->resizeColumnsToContents();
        break;

    case MainWindow::TAB::TEACHER:
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
}

void MainWindow::deleteRows(){
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->tabs_->currentIndex());
    QItemSelectionModel *selectionModel = this->tableView_[this->tabs_->currentIndex()]->selectionModel();
    if(!selectionModel->hasSelection()){
        QMessageBox::warning(this, tr("No row selected"), tr("Please select at least one row"));
        return;
    }
   QModelIndexList indexList = selectionModel->selectedRows();
    switch(tab){
    case MainWindow::TAB::STUDENT:
       for(QModelIndex index : indexList){
           this->studentTableModel_->removeRows(index.row(), 1);
       }
        break;

    case MainWindow::TAB::TEACHER:
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
}

void MainWindow::openFilterDialog(){

}
