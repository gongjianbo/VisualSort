#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <QObject>
#include <QTimer>
#include <QTimeLine>

#include "SortObejct.h"

//冒泡排序
class BubbleSort : public SortObejct
{
    Q_OBJECT
public:
    explicit BubbleSort(QObject *parent = nullptr);

    void runStart(int interval) override;
    void runStep() override;
    bool isFinish() const override;
    void paint(QPainter *painter, int width, int height) override;

private:
    bool _isFinish=true;
    int _i=0;
    int _j=0;
    int _jTemp=0;
    int _swapIndex=0;

    QTimer _sortTimer; //用于排序

    double _animationProgress=0;
    QTimeLine _animationLine; //用于动画
};

#endif // BUBBLESORT_H
