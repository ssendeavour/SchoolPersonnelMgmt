#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>
#include <QDialog>
#include <QLocale>

#include "CommonTableDelegate.h"
#include "CommonFilterDialog.h"

#include "StudentTableModel.h"
#include "TeacherTableModel.h"



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

    void openFilterDialog();

protected:
    bool openStudentFile(QFile &file, QString &error);
    bool saveStudentFile(QFile &file, QString &error);

    bool openTeacherFile(QFile &file, QString &error);
    bool saveTeacherFile(QFile &file, QString &error);

private:
    void initUI();
    void initStudentTab();
    void initTeacherTab();
//    void initPostgraduateTab();
//    void initTeachingAssistantTab();

    void addMenuBarToolBar();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs_;

    QTableView *tableView_[TAB_NUMBER];

    // save old editTriggers when show filter dialog, and disable edit when
    // filtering, for changes will lost when close filter dialog
    // restore  editTriggers on dialog close
    QAbstractItemView::EditTriggers editTrigger_;

    StudentTableModel *studentTableModel_;
    TeacherTableModel *teacherTableModel_;

    // this dialog is not added to MainWindow as it's child, so that a icon will show in taskbar
    CommonFilterDialog *filterDialog_ = nullptr;
};

