#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int STUDENT = 0;
    static const int TEACHER = 1;

public slots:
    bool openFile();
    bool saveFile();
    bool clearAllData();

protected:

private:
    void initUI();

    void addMenuBarToolBar();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs;
};

