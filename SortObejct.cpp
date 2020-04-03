#include "SortObejct.h"

#include <algorithm>

SortObejct::SortObejct(QObject *parent) : QObject(parent)
{

}

void SortObejct::dataReset(int count)
{
    if(count<2)
        return;

    //初始化并随机打乱数据
    _sortData.resize(count);
    for(int i=0;i<count;i++)
    {
        _sortData[i]=i+1;
    }
    std::random_shuffle(_sortData.begin(),_sortData.end());

    emit sortUpdated();
}
