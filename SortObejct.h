#ifndef SORTOBEJCT_H
#define SORTOBEJCT_H

#include <QObject>
#include <QVector>
#include <QPainter>

//排序的接口类
class SortObejct : public QObject
{
    Q_OBJECT
public:
    explicit SortObejct(QObject *parent = nullptr);

    void dataReset(int count);

    virtual void runStart(int interval)=0;
    virtual void runStep()=0;
    virtual bool isFinish() const=0;
    virtual void paint(QPainter *painter,int width,int height)=0;

signals:
    void sortUpdated();
    void sortFinished();

protected:
    QVector<int> _sortData;
};

#endif // SORTOBEJCT_H
