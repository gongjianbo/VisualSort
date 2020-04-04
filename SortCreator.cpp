#include "SortCreator.h"

#include "BubbleSort.h"
#include "SelectionSort.h"

SortCreator::SortCreator(QObject *parent) : QObject(parent)
{

}

SortObejct *SortCreator::createSortObject(int row, QObject *parent)
{
    switch (row) {
    case 0: return new BubbleSort(parent);
    case 1: return new SelectionSort(parent);
    default:
        break;
    }
    return nullptr;
}

QStringList SortCreator::getSortList()
{
    return {"冒泡排序","选择排序"};
}
