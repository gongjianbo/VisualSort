#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "BubbleSort.h"

#include <QButtonGroup>
#include <QListView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSettingBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSettingBar()
{
    //互斥
    QButtonGroup *group=new QButtonGroup(this);
    group->addButton(ui->btnReset);
    group->addButton(ui->btnSort);

    //下拉框item行高
    ui->comboType->setView(new QListView(this));

    //重置数据
    connect(ui->btnReset,&QPushButton::clicked,this,[this]{
        ui->canvas->dataReset(new BubbleSort(ui->canvas),ui->spinCount->value());
    });
    //排序开始
    connect(ui->btnSort,&QPushButton::clicked,this,[this](bool checked){
        ui->comboType->setEnabled(!checked);
        ui->spinCount->setEnabled(!checked);
        ui->spinInterval->setEnabled(!checked);
        ui->canvas->startSort(ui->spinInterval->value());
    });
    //排序完
    connect(ui->canvas,&SortCanvas::finished,this,[this]{
        ui->btnReset->setChecked(true);
    });
}

