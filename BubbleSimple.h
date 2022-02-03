#pragma once
#include <QWidget>
#include <QVector>
#include <QEventLoop>
#include <QPropertyAnimation>

//使用冒泡排序对动画逻辑进行测试
//龚建波 2022-2-2
class BubbleSimple : public QWidget
{
    Q_OBJECT
    //配合属性动画来控制交换元素的动画效果
    Q_PROPERTY(double offset READ getOffset WRITE setOffset NOTIFY offsetChanged)
public:
    explicit BubbleSimple(QWidget *parent = nullptr);

    bool getRunFlag() const;
    void setRunFlag(bool flag);

    double getOffset() const;
    void setOffset(double offset);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initArr();
    void run();
    void stop();

signals:
    void runFlagChanged(bool running);
    void offsetChanged(double offset);

private:
    //=false时退出for循环
    bool runFlag{false};
    QEventLoop loop;
    QPropertyAnimation animation;

    QVector<int> arr;
    //冒泡for循环下标
    int arrI{0};
    int arrJ{0};
    //标记当前交换状态
    bool swapFlag{false};
    //交换进度
    double swapOffset{0.0};
};
