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


#include "const.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs_(new QTabWidget(this))
//    filterDialog(nullptr)
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
    initTeacherTab();
//    initPostgraduateTab();
//    initTeachingAssistantTab();

    this->tabs_->addTab(this->tableView_[MainWindow::TAB::STUDENT], tr("&Student"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::TEACHER], tr("&Teacher"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::POSTGRAD], tr("&Postgraduate"));
    this->tabs_->addTab(this->tableView_[MainWindow::TAB::TA], tr("Teaching &Assistant"));
    setCentralWidget(this->tabs_);
}

void MainWindow::initStudentTab() {
    const int index = static_cast<int>(MainWindow::TAB::STUDENT);
    QVector<CONST::HDG> headerIndexMap(6);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::CLASSNO;
    headerIndexMap[3] = CONST::HDG::SEX;
    headerIndexMap[4] = CONST::HDG::BIRTHDAY;
    headerIndexMap[5] = CONST::HDG::IDNUMBER;
    this->studentTableModel_ = new StudentTableModel(headerIndexMap, this);
    this->tableView_[index]->setModel(this->studentTableModel_);
    this->tableView_[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->tableView_[index]));
    this->tableView_[index]->resizeColumnsToContents();
}

void MainWindow::initTeacherTab() {
    const int index = static_cast<int>(MainWindow::TAB::TEACHER);
    QVector<CONST::HDG> headerIndexMap(7);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::BIRTHDAY;
    headerIndexMap[4] = CONST::HDG::DEPT;
    headerIndexMap[5] = CONST::HDG::POSITION;
    headerIndexMap[6] = CONST::HDG::IDNUMBER;
    this->teacherTableModel_ = new TeacherTableModel(headerIndexMap, this);
    this->tableView_[index]->setModel(this->teacherTableModel_);
    this->tableView_[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->tableView_[index]));
    this->tableView_[index]->resizeColumnsToContents();
}

/*
void MainWindow::initPostgraduateTab(){
    const int index = static_cast<int>(MainWindow::TAB::STUDENT);
    QStringList stuTableHeader;
    QVector<CONST::HDG> headerIndexMap(5);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::BIRTHDAY;
    headerIndexMap[4] = CONST::HDG::IDNUMBER;
    this->studentTableModel_ = new StudentTableModel(headerIndexMap, this);
    this->tableView_[index]->setModel(this->studentTableModel_);
    this->tableView_[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->tableView_[index]));
    this->tableView_[index]->resizeColumnsToContents();
}

void MainWindow::initTeachingAssistantTab(){
    const int index = static_cast<int>(MainWindow::TAB::STUDENT);
    QStringList stuTableHeader;
    QVector<CONST::HDG> headerIndexMap(5);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::BIRTHDAY;
    headerIndexMap[4] = CONST::HDG::IDNUMBER;
    this->studentTableModel_ = new StudentTableModel(headerIndexMap, this);
    this->tableView_[index]->setModel(this->studentTableModel_);
    this->tableView_[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->tableView_[index]));
    this->tableView_[index]->resizeColumnsToContents();
}
*/

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

//    editMenu->addSeparator();

    QAction *filterAction = new QAction(tr("&Find && Filter"), this);
    filterAction->setIconText(tr("Find"));
    filterAction->setShortcut(QKeySequence::Find);
    connect(filterAction, &QAction::triggered, this, &MainWindow::openFilterDialog);
    editMenu->addAction(filterAction);

    editMenu->addSeparator();

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
    ui->mainToolBar->addAction(filterAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(quitAction);

    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
}

bool MainWindow::openFile()
{
    int index = this->tabs_->currentIndex();
    bool succeed = true;
    QString fileFormatString;
    QString ext;
    switch(static_cast<MainWindow::TAB>(index)){
    case MainWindow::TAB::STUDENT:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Student").arg(CONST::FILE_EXTENSION_STUDENT);
        ext = CONST::FILE_EXTENSION_STUDENT;
        break;
    case MainWindow::TAB::TEACHER:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teacher").arg(CONST::FILE_EXTENSION_TEACHER);
        ext = CONST::FILE_EXTENSION_TEACHER;
        break;
    case MainWindow::TAB::POSTGRAD:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Postgraduate").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
        ext = CONST::FILE_EXTENSION_POSTGRADUATE;
        break;
    case MainWindow::TAB::TA:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teaching Assistant").arg(CONST::FILE_EXTENSION_TA);
        ext = CONST::FILE_EXTENSION_TA;
        break;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), ".", fileFormatString);
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
    case MainWindow::TAB::TEACHER: {
        succeed = openTeacherFile(file, error);
        break;
    }
    case MainWindow::TAB::POSTGRAD: {

        break;
    }
    case MainWindow::TAB::TA: {

        break;
    }
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
    int index = this->tabs_->currentIndex();
    QString fileFormatString;
    QString ext;
    switch(static_cast<MainWindow::TAB>(index)){
    case MainWindow::TAB::STUDENT:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Student").arg(CONST::FILE_EXTENSION_STUDENT);
        ext = CONST::FILE_EXTENSION_STUDENT;
        break;
    case MainWindow::TAB::TEACHER:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teacher").arg(CONST::FILE_EXTENSION_TEACHER);
        ext = CONST::FILE_EXTENSION_TEACHER;
        break;
    case MainWindow::TAB::POSTGRAD:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Postgraduate").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
        ext = CONST::FILE_EXTENSION_POSTGRADUATE;
        break;
    case MainWindow::TAB::TA:
        fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teaching Assistant").arg(CONST::FILE_EXTENSION_TA);
        ext = CONST::FILE_EXTENSION_TA;
        break;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), ".", fileFormatString);
    if(fileName.isEmpty()){
        return false;
    }
    // add extension if necessary
    if(!fileName.endsWith("." + ext)){
        fileName +=  "." + ext;
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
        break;
    }
    case MainWindow::TAB::TEACHER: {
        succeed = saveTeacherFile(file, error);
        break;
    }
    case MainWindow::TAB::POSTGRAD: {

        break;
    }
    case MainWindow::TAB::TA: {

        break;
    }
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
// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openTeacherFile(QFile &file, QString &error){
    QList<Teacher> list = Teacher::readFromFile(file, error);
    if(error.length() > 0){
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }
    this->teacherTableModel_->setDataList(list);
    return true;
}

bool MainWindow::saveTeacherFile(QFile &file, QString &error){
    if(!Teacher::writeToFile(file, this->teacherTableModel_->getDataList())){
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
        this->teacherTableModel_->setDataList(QList<Teacher>());
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
        this->teacherTableModel_->insertRows(this->teacherTableModel_->rowCount(), 1);
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
    this->tableView_[tabIndex]->resizeColumnsToContents();
    this->tableView_[tabIndex]->scrollToBottom();
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
            rowCount = this->teacherTableModel_->rowCount();
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
        break;
    case MainWindow::TAB::TEACHER:
        this->teacherTableModel_->insertRows(selectionModel->selectedRows().at(0).row(), 1);
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
    this->tableView_[tabIndex]->resizeColumnsToContents();
}

void MainWindow::deleteRows(){
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->tabs_->currentIndex());
    QItemSelectionModel *selectionModel = this->tableView_[this->tabs_->currentIndex()]->selectionModel();
    if(!selectionModel->hasSelection()){
        QMessageBox::warning(this, tr("No row selected"), tr("Please select at least one row"));
        return;
    }
   QModelIndexList indexList = selectionModel->selectedRows();
   // sort selected row from high index to low index, and delete them in the same order
   qSort(indexList.begin(), indexList.end(), [](QModelIndex &left, QModelIndex &right){return left.row() >= right.row();});
   // detect consecutive selections
   QList<QPair<int, int>> pairs = QList<QPair<int, int>>();
   int start = indexList.at(0).row();
   int current = start;
   for(int i=0; i < indexList.size(); ++i){
       int row = indexList.at(i).row();
       if(row - current <= 0 && row - current >= -1){
           current = row;
       } else {
           pairs.append(QPair<int, int>(current, start - current + 1));
           start = row;
           current = start;
       }
   }
   pairs.append(QPair<int, int>(current, start - current + 1));

    switch(tab){
    case MainWindow::TAB::STUDENT: {
        for(QPair<int, int> pair : pairs){
            this->studentTableModel_->removeRows(pair.first, pair.second);
        }
        break;
    }
    case MainWindow::TAB::TEACHER: {
        for(QPair<int, int> pair : pairs) {
            this->teacherTableModel_->removeRows(pair.first, pair.second);
        }
        break;
    }
    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
}

void MainWindow::openFilterDialog(){
    QVector<CONST::HDG> headingIndex;
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->tabs_->currentIndex());
    switch(tab){
    case MainWindow::TAB::STUDENT: {
        headingIndex = this->studentTableModel_->getHeaderIndexs();
        // don't specify a parent, so that this dialog has its own icon in TaskBar area
        this->filterDialog_ = new CommonFilterDialog(headingIndex);
        this->filterDialog_->setAttribute(Qt::WA_DeleteOnClose);
        connect(this->filterDialog_, &CommonFilterDialog::filterColumnChanged, this->studentTableModel_, &StudentTableModel::setFilterColumn);
        connect(this->filterDialog_, &CommonFilterDialog::filterTextChanged, this->studentTableModel_, &StudentTableModel::setFilterString);
        connect(this->filterDialog_, &CommonFilterDialog::useRegExp, this->studentTableModel_, &StudentTableModel::setFilterUseRegexp);
        connect(this->filterDialog_, &CommonFilterDialog::caseSensitivityChanged, this->studentTableModel_, &StudentTableModel::setFilterCaseSentivity);
        connect(this->filterDialog_, &CommonFilterDialog::sexTypeChanged, this->studentTableModel_, &StudentTableModel::setFilterSex);
        connect(this->filterDialog_, &CommonFilterDialog::fromBirthdayChanged, this->studentTableModel_, &StudentTableModel::setFilterMinDate);
        connect(this->filterDialog_, &CommonFilterDialog::toBirthdayChanged, this->studentTableModel_, &StudentTableModel::setFilterMaxDate);
        connect(this->filterDialog_, &CommonFilterDialog::finished, [this](){ this->studentTableModel_->setEnableFilter(false); });
        this->studentTableModel_->setEnableFilter(true);
        this->studentTableModel_->setFilterCaseSentivity(false);
        this->studentTableModel_->setFilterUseRegexp(false);
        this->studentTableModel_->setFilterColumn(CONST::HDG::ID);
        this->studentTableModel_->setFilterMinDate(QDate(1880,1,1));
        this->studentTableModel_->setFilterMaxDate(QDate::currentDate());
        this->studentTableModel_->setFilterSex(Person::Sex::Male);
        this->studentTableModel_->setFilterString("");
        break;
    }
    case MainWindow::TAB::TEACHER:
        headingIndex = this->teacherTableModel_->getHeaderIndexs();
        // don't specify a parent, so that this dialog has its own icon in TaskBar area
        this->filterDialog_ = new CommonFilterDialog(headingIndex);
        this->filterDialog_->setAttribute(Qt::WA_DeleteOnClose);
        connect(this->filterDialog_, &CommonFilterDialog::filterColumnChanged, this->teacherTableModel_, &TeacherTableModel::setFilterColumn);
        connect(this->filterDialog_, &CommonFilterDialog::filterTextChanged, this->teacherTableModel_, &TeacherTableModel::setFilterString);
        connect(this->filterDialog_, &CommonFilterDialog::useRegExp, this->teacherTableModel_, &TeacherTableModel::setFilterUseRegexp);
        connect(this->filterDialog_, &CommonFilterDialog::caseSensitivityChanged, this->teacherTableModel_, &TeacherTableModel::setFilterCaseSentivity);
        connect(this->filterDialog_, &CommonFilterDialog::sexTypeChanged, this->teacherTableModel_, &TeacherTableModel::setFilterSex);
        connect(this->filterDialog_, &CommonFilterDialog::fromBirthdayChanged, this->teacherTableModel_, &TeacherTableModel::setFilterMinDate);
        connect(this->filterDialog_, &CommonFilterDialog::toBirthdayChanged, this->teacherTableModel_, &TeacherTableModel::setFilterMaxDate);
        connect(this->filterDialog_, &CommonFilterDialog::finished, [this](){ this->teacherTableModel_->setEnableFilter(false); });
        this->teacherTableModel_->setEnableFilter(true);
        this->teacherTableModel_->setFilterCaseSentivity(false);
        this->teacherTableModel_->setFilterUseRegexp(false);
        this->teacherTableModel_->setFilterColumn(CONST::HDG::ID);
        this->teacherTableModel_->setFilterMinDate(QDate(1880,1,1));
        this->teacherTableModel_->setFilterMaxDate(QDate::currentDate());
        this->teacherTableModel_->setFilterSex(Person::Sex::Male);
        this->teacherTableModel_->setFilterString("");
        break;

    case MainWindow::TAB::POSTGRAD:
        break;

    case MainWindow::TAB::TA:
        break;
    }
    // show as non-model dialog
    this->editTrigger_ = this->tableView_[this->tabs_->currentIndex()]->editTriggers();
    this->tableView_[this->tabs_->currentIndex()]->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this->filterDialog_, &CommonFilterDialog::finished, [this](){
        this->tableView_[this->tabs_->currentIndex()]->setEditTriggers(this->editTrigger_);
    });
    this->filterDialog_->show();
}
