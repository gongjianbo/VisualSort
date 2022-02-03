#include "MainUI.h"
#include "ui_MainUI.h"
#include "SortFactory.h"
#include "SortObject.h"

#include <QListView>

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    init();
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::init()
{
    //下拉框item行高
    ui->comboType->setView(new QListView(this));
    //排序类型
    ui->comboType->addItems(SortFactory::getInstance()->getSortList());

    //点击开始排序
    connect(ui->btnSort, &QPushButton::clicked, this, [this]{
        const int type = ui->comboType->currentIndex();
        if (type != ui->canvas->getSortType()) {
            SortObject *obj = SortFactory::getInstance()->createSortObject(type, ui->canvas);
            ui->canvas->setSortObject(type, obj);
        }
        ui->canvas->sort(ui->spinCount->value(), ui->spinInterval->value());
    });

    //点击结束排序
    connect(ui->btnStop, &QPushButton::clicked, this, [this]{
        ui->canvas->stop();
    });

    //排序状态，排序时不能修改参数
    connect(ui->canvas, &MainCanvas::runFlagChanged,
            this, [this](bool running){
        ui->comboType->setEnabled(!running);
        ui->spinCount->setEnabled(!running);
        ui->spinInterval->setEnabled(!running);
        ui->btnSort->setEnabled(!running);
    });
}
