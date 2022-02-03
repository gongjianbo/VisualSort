#include "SortObject.h"

SortObject::SortObject(QObject *parent)
    : QObject(parent)
{

}

bool SortObject::getRunFlag() const
{
    return runFlag;
}

void SortObject::setRunFlag(bool flag)
{
    if (runFlag != flag) {
        runFlag = flag;
        emit runFlagChanged(flag);
    }
}
