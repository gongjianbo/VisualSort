#ifndef SORTCREATOR_H
#define SORTCREATOR_H

#include <QObject>

#include "SortObejct.h"

class SortCreator : public QObject
{
    Q_OBJECT
public:
    explicit SortCreator(QObject *parent = nullptr);

    static SortObejct *createSortObject(int row,QObject *parent);
    static QStringList getSortList();
};

#endif // SORTCREATOR_H
