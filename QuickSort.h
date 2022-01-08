#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QTimeLine>
#include <QMutex>
#include <QWaitCondition>

#include "SortObejct.h"

//快速排序
//半成品：遍历过程和交换动画都还没写
class QuickSort : public SortObejct
{
    Q_OBJECT
public:
    explicit QuickSort(QObject *parent = nullptr);
    ~QuickSort();

    void runStart(int interval) override;
    void runStep() override;
    void paint(QPainter *painter, int width, int height) override;

private:
    //这里用挖坑法
    void doSort(int left,int right);
    int doSortPart(int left,int right);

private:
    //因为目前datareset是初始化时进行的，所以没有对其锁定
    QMutex _waitMutex;
    QWaitCondition _waitCondition;
    std::atomic<bool> _runningFlag{false}; //是否正在排序

    int _interval{1}; //操作定时间隔
    QTimer _sortTimer; //排序步骤定时器

    int _leftTemp{0}; //左边界
    int _rightTemp{0}; //右边界
    int _beginTemp{0}; //左指针
    int _endTemp{0}; //右指针
    int _pitTemp{0}; //坑位

    double _animationProgress{0};
    QTimeLine _animationLine; //用于动画
};

#endif // QUICKSORT_H
