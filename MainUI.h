#pragma once
#include <QMainWindow>

namespace Ui {
class MainUI;
}

//主窗口
//龚建波 2022-2-3
class MainUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainUI(QWidget *parent = nullptr);
    ~MainUI();

private:
    void init();

private:
    Ui::MainUI *ui;
};
