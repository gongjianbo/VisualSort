#pragma once
#include "SortObject.h"
#include <QVector>
#include <QEventLoop>
#include <QVariantAnimation>

//希尔排序
//龚建波 2022-2-12
class ShellSort : public SortObject
{
public:
    explicit ShellSort(QObject *parent = nullptr);

    //开始排序
    //count元素个数，interval动画持续时间参考值
    void sort(int count, int interval) override;
    //结束排序
    void stop() override;
    //绘制
    void draw(QPainter *painter, int width, int height) override;

private:
    void initArr(int count);

private:
    QEventLoop loop;
    QVariantAnimation animation;

    QVector<int> arr;
    //for循环下标
    int arrI{0};
    int arrJ{0};
    int arrGap{0};
    //标记当前交换状态
    bool swapFlag{false};
};
