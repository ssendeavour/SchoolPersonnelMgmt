#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>

#include "StudentTableModel.h"
#include "StudentTableDelegate.h"
#include "CommonSortFilterProxyModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int TAB_NUMBER = 4;

    enum TAB {
        STUDENT = 0,
        TEACHER = 1,
        POSTGRAD = 2,
        TA = 3
    };

public slots:
    bool openFile();
    bool saveFile();
    void clearAllData();
    void addNewRow();
    void insertRowBefore();
    void deleteRows();

protected:
    bool openStudentFile(QFile &file, QString &error);
    bool saveStudentFile(QFile &file, QString &error);

private:
    void initUI();
    void initStudentTab();

    void addMenuBarToolBar();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs_;
    // pointer to array of pointers
    QTableView *tableView_[TAB_NUMBER];
    CommonSortFilterProxyModel *sortFilterProxyModel_[TAB_NUMBER];

    StudentTableModel *studentTableModel_;
};

