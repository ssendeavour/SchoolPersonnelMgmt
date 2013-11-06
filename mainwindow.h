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

private:
    void initUI();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs;

};

