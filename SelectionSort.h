#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <QObject>
#include <QTimer>
#include <QTimeLine>

#include "SortObejct.h"

//选择排序
class SelectionSort : public SortObejct
{
    Q_OBJECT
public:
    explicit SelectionSort(QObject *parent = nullptr);

    void runStart(int interval) override;
    void runStep() override;
    bool isFinish() const override;
    void paint(QPainter *painter, int width, int height) override;

private:
    bool _isFinish=true;
    //目前排序时一次性的，所以没有复位操作
    int _i=0;
    int _j=1;
    int _min=0;
    int _iTemp=0;
    int _jTemp=1;
    int _minTemp=0;

    QTimer _sortTimer; //用于排序

    double _animationProgress=0;
    QTimeLine _animationLine; //用于动画
};

#endif // SELECTIONSORT_H
