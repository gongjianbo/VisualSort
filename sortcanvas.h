#ifndef SORTCANVAS_H
#define SORTCANVAS_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

#include "SortObejct.h"

namespace Ui {
class SortCanvas;
}

class SortCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit SortCanvas(QWidget *parent = nullptr);
    ~SortCanvas();

    void dataReset(SortObejct *sortObj,int count);
    void startSort(int interval);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void finished();

private:
    Ui::SortCanvas *ui;


    SortObejct *_sortObj=nullptr;
};

#endif // SORTCANVAS_H
