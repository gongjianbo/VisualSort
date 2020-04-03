#include "sortcanvas.h"
#include "ui_sortcanvas.h"

#include <QTime>
#include <QDebug>

SortCanvas::SortCanvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SortCanvas)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    //QTime time=QTime::currentTime();
    //qsrand(time.msec()+time.second()*1000);
}

SortCanvas::~SortCanvas()
{
    delete ui;
}

void SortCanvas::dataReset(SortObejct *sortObj, int count)
{
    //重置数据，目前sortobj是一次性的
    if(_sortObj){
        _sortObj->deleteLater();
        _sortObj=nullptr;
    }
    _sortObj=sortObj;
    if(_sortObj){
        connect(_sortObj,&SortObejct::sortUpdated,this,[this]{
            update();
        });
        connect(_sortObj,&SortObejct::sortFinished,this,[this]{
            update();
            emit finished();
        });
        _sortObj->dataReset(count);
    }
    update();//填充好数据刷新
}

void SortCanvas::startSort(int interval)
{
    //开始排序
    if(_sortObj){
        _sortObj->runStart(interval);
    }else{
        emit finished();
    }
    update();
}

void SortCanvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(),QColor(20,20,20));

    if(_sortObj){
        _sortObj->paint(&painter,width(),height());
    }
}
