#include "SortCreator.h"

#include "BubbleSort.h"

SortCreator::SortCreator(QObject *parent) : QObject(parent)
{

}

SortObejct *SortCreator::createSortObject(int row, QObject *parent)
{
    switch (row) {
    case 0: return new BubbleSort(parent);
    default:
        break;
    }
    return nullptr;
}

QStringList SortCreator::getSortList()
{
    return {"冒泡排序"};
}
