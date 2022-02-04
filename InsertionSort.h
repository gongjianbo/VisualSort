#pragma once
#include "SortObject.h"
#include <QVector>
#include <QEventLoop>
#include <QPropertyAnimation>

//插入排序
//龚建波 2022-2-4
class InsertionSort : public SortObject
{
    Q_OBJECT
    //配合属性动画来控制交换元素的动画效果
    Q_PROPERTY(double offset READ getOffset WRITE setOffset NOTIFY offsetChanged)
public:
    explicit InsertionSort(QObject *parent = nullptr);

    double getOffset() const;
    void setOffset(double offset);

    //开始排序
    //count元素个数，interval动画持续时间参考值
    void sort(int count, int interval) override;
    //结束排序
    void stop() override;
    //绘制
    void draw(QPainter *painter, int width, int height) override;

private:
    void initArr(int count);

signals:
    void offsetChanged(double offset);

private:
    QEventLoop loop;
    QPropertyAnimation animation;

    QVector<int> arr;
    //for循环下标
    int arrI{0};
    int arrJ{0};
    //标记当前交换状态
    bool swapFlag{false};
    //交换进度
    double swapOffset{0.0};
};