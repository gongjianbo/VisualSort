#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SortCreator.h"

#include <QButtonGroup>
#include <QListView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<"C++/Python/Qt qq交流群：647637553";

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
    //排序类型
    ui->comboType->addItems(SortCreator::getSortList());

    //重置数据
    connect(ui->btnReset,&QPushButton::clicked,this,[this]{
        //目前没有比较已有的类型是否相同
        SortObejct *obj=SortCreator::createSortObject(ui->comboType->currentIndex(),ui->canvas);
        ui->canvas->dataReset(obj,ui->spinCount->value());
    });
    //排序开始
    connect(ui->btnSort,&QPushButton::clicked,this,[this]{
        ui->canvas->startSort(ui->spinInterval->value());
    });
    connect(ui->btnSort,&QPushButton::toggled,this,[this](bool checked){
        //在排序的时候不能设置参数
        ui->comboType->setEnabled(!checked);
        ui->spinCount->setEnabled(!checked);
        ui->spinInterval->setEnabled(!checked);
    });
    //排序完
    connect(ui->canvas,&SortCanvas::finished,this,[this]{
        ui->btnReset->setChecked(true);
    });
}

