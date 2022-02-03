#pragma once
#include <QObject>
#include "SortObject.h"

//生成SortObject
//龚建波 2022-2-3
class SortFactory : public QObject
{
    Q_OBJECT
private:
    explicit SortFactory(QObject *parent = nullptr);
public:
    static SortFactory *getInstance();

    //创建一个排序对象
    //row对应sortlist列表的排序方式
    SortObject *createSortObject(int row, QObject *parent);

    //返回排序方式列表，作为combobox选项
    QStringList getSortList() const;
};
