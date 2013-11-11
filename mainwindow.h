#pragma once

#include <QMainWindow>
#include <QTabWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

