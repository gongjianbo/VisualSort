#include "SortFactory.h"
#include "BubbleSort.h"
#include "SelectionSort.h"

SortFactory::SortFactory(QObject *parent)
    : QObject(parent)
{

}

SortFactory *SortFactory::getInstance()
{
    static SortFactory instance;
    return &instance;
}

SortObject *SortFactory::createSortObject(int row, QObject *parent)
{
    switch (row) {
    case 0: return new BubbleSort(parent);
    case 1: return new SelectionSort(parent);
    default: break;
    }
    return nullptr;
}

QStringList SortFactory::getSortList() const
{
    return QStringList{
        "冒泡排序"
        , "选择排序"
    };
}
