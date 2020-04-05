#include "QuickSort.h"

#include <QMutexLocker>
#include <QtConcurrentRun>
#include <QThread>
#include <QDebug>

QuickSort::QuickSort(QObject *parent)
    : SortObejct(parent)
{
    //排序的定时器
    connect(&_sortTimer,&QTimer::timeout,this,[this]{
        if(isFinish()){
            _sortTimer.stop();
            emit sortFinished();
        }else{
            _waitMutex.lock();
            _waitCondition.wakeAll();
            _waitMutex.unlock();
        }
    });

    //动画
    _animationLine.setFrameRange(0,100);
    connect(&_animationLine,&QTimeLine::frameChanged,this,[this](int frame){
        _animationProgress=frame/100.0;
        emit sortUpdated();
    });
}

QuickSort::~QuickSort()
{
    if(_runningFlag){
        _runningFlag=false;
        _waitMutex.lock();
        _waitCondition.wakeAll();
        //按理来说我的流程是对的，但还是报错：
        //QWaitCondition: Destroyed while threads are still waiting
        //所以我干脆在这里加了一句带超时的wait
        _waitCondition.wait(&_waitMutex,10);
        _waitMutex.unlock();
    }
}

void QuickSort::runStart(int interval)
{
    setFinish(false);
    _interval=interval;
    runStep();
}

void QuickSort::runStep()
{
    //QFuture<void> future=
    QtConcurrent::run([this]{
        _runningFlag=true;

#if 0
        //用冒泡排序测试
        const int data_count=_sortData.count();
        for (int i=0; i<data_count-1; i++)
        {
            for (int j=0; j<data_count-1-i; j++)
            {

                _iTemp=j;
                _jTemp=j+1;
                _waitMutex.lock();
                _waitCondition.wait(&_waitMutex);
                if(!_runningFlag){
                    _waitMutex.unlock();
                    return;
                }
                if (_sortData[j]>_sortData[j+1]){
                    std::swap(_sortData[j],_sortData[j+1]);
                }
                _waitMutex.unlock();
                emit sortUpdated();
            }
        }
#endif
        doSort(0,_sortData.count()-1);
        _runningFlag=false;
        setFinish(true);
    });
    _sortTimer.start(_interval);
}

void QuickSort::paint(QPainter *painter, int width, int height)
{
    QMutexLocker locker(&_waitMutex);
    //柱状图与边框距离
    const int left_space=0;
    const int right_space=0;
    const int top_space=20;
    const int bottom_space=100;

    const int data_count=_sortData.count();
    const int item_space=10;//横项间隔
    const int text_height=painter->fontMetrics().height();
    const int text_space=15;
    const double item_width=(width+item_space-left_space-right_space)/(double)data_count-item_space;
    const double item_bottom=height-bottom_space;
    const double height_factor=(height-top_space-bottom_space)/(double)data_count;
    double item_left=0;
    double item_height=0;

    painter->setPen(QColor(200,200,200));
    painter->drawText(0,text_height,"快速排序：挖坑法演示（半成品）");

    if(!isFinish()&&_leftTemp<data_count&&_rightTemp<data_count
            &&_beginTemp<data_count&&_endTemp<data_count
            &&_pitTemp<data_count){
        QColor item_color;
        for(int i=0;i<data_count;i++)
        {
            item_left=left_space+i*(item_width+item_space);
            item_height=height_factor*_sortData.at(i);
            item_color=(i>=_leftTemp&&i<=_rightTemp)?QColor(230,230,200):QColor(200,200,200);
            painter->fillRect(item_left,item_bottom-item_height,
                              item_width,item_height,
                              item_color);
            /*if(i==_pitTemp){
                painter->fillRect(item_left,item_bottom-item_height,
                                  item_width,item_height,
                                  QColor(0,200,0));

            }*/
        }

        const double item_left_1=left_space+_beginTemp*(item_width+item_space);
        const double item_left_2=left_space+_endTemp*(item_width+item_space);
        const double item_height_1=height_factor*_sortData.at(_beginTemp);
        const double item_height_2=height_factor*_sortData.at(_endTemp);

        //begin
        painter->fillRect(item_left_1,item_bottom-item_height_1,
                          item_width,item_height_1,
                          QColor(250,170,0));
        const QString item_text_1="begin->";//QString::number(_sortData.at(_beginTemp));
        painter->drawText(item_left_1
                          +(item_width-painter->fontMetrics().width(item_text_1))/2,
                          item_bottom+(text_height+text_space)*1,
                          item_text_1);
        //end
        painter->fillRect(item_left_2,item_bottom-item_height_2,
                          item_width,item_height_2,
                          QColor(250,170,0));
        const QString item_text_2="<-end";//QString::number(_sortData.at(_endTemp));
        painter->drawText(item_left_2
                          +(item_width-painter->fontMetrics().width(item_text_2))/2,
                          item_bottom+(text_height+text_space)*2,
                          item_text_2);

        //坑位
        const double pit_left=left_space+_pitTemp*(item_width+item_space);
        const double pit_top=item_bottom+(text_height+text_space)*2;
        QPainterPath pit_path;
        pit_path.moveTo(pit_left+item_width/2.0,pit_top);
        pit_path.lineTo(pit_left,pit_top+item_width/2.0);
        pit_path.lineTo(pit_left+item_width,pit_top+item_width/2.0);
        pit_path.lineTo(pit_left+item_width/2.0,pit_top);
        painter->fillPath(pit_path,QColor(250,170,0));

        //当前区间
        const double line_left=left_space+_leftTemp*(item_width+item_space);
        const double line_top=item_bottom+(text_height+text_space)*3;
        const double line_width=(_rightTemp-_leftTemp)*(item_width+item_space)+item_width;
        painter->fillRect(line_left,line_top,line_width,5,QColor(230,230,200));
    }else{
        //绘制所有的数据条
        for(int i=0;i<data_count;i++)
        {
            item_left=left_space+i*(item_width+item_space);
            item_height=height_factor*_sortData.at(i);
            painter->fillRect(item_left,item_bottom-item_height,
                              item_width,item_height,
                              QColor(200,200,200));
        }
    }
}

void QuickSort::doSort(int left, int right)
{
    if(!_runningFlag){
        return;
    }
    if(left>right)
        return;

    const int pos = doSortPart(left,right);
    if(!_runningFlag){
        return;
    }
    doSort(left, pos - 1);
    doSort(pos+1, right);
}

int QuickSort::doSortPart(int left, int right)
{
    if(!_runningFlag){
        return 0;
    }

    _waitMutex.lock();
    int begin = left; //左边界
    int end = right;  //右边界
    int temp = _sortData[begin]; //begin是第一个坑
    _leftTemp=begin;
    _rightTemp=end;
    _beginTemp=begin;
    _endTemp=end;
    _pitTemp=begin;

    //qDebug()<<"while"<<begin<<end<<_sortData;
    while(begin < end){
        //从右边找起，找比temp小的来填坑
        while (begin < end && _sortData[end] >= temp){
            {
                _beginTemp=begin;
                _endTemp=end;
                emit sortUpdated();
                _waitCondition.wait(&_waitMutex);
                if(!_runningFlag){
                    _waitMutex.unlock();
                    return 0;
                }
            }
            end--;
        }

        {
            _beginTemp=begin;
            _endTemp=end;
            emit sortUpdated();
            _waitCondition.wait(&_waitMutex);
            if(!_runningFlag){
                _waitMutex.unlock();
                return 0;
            }
        }
        //end成为新坑
        if (begin < end){
            _sortData[begin++] = _sortData[end];
        }
        {
            _beginTemp=begin;
            _endTemp=end;
            _pitTemp=end;
            emit sortUpdated();
            _waitCondition.wait(&_waitMutex);
            if(!_runningFlag){
                _waitMutex.unlock();
                return 0;
            }
        }

        //再从左边找比temp大的
        while(begin < end && _sortData[begin] <= temp){
            {
                _beginTemp=begin;
                _endTemp=end;
                emit sortUpdated();
                _waitCondition.wait(&_waitMutex);
                if(!_runningFlag){
                    _waitMutex.unlock();
                    return 0;
                }
            }

            begin++;
        }
        {
            _beginTemp=begin;
            _endTemp=end;
            emit sortUpdated();
            _waitCondition.wait(&_waitMutex);
            if(!_runningFlag){
                _waitMutex.unlock();
                return 0;
            }
        }
        //begin成为新坑
        if (begin < end){
            _sortData[end--] = _sortData[begin];
        }
        {
            _beginTemp=begin;
            _endTemp=end;
            _pitTemp=begin;
            emit sortUpdated();
            _waitCondition.wait(&_waitMutex);
            if(!_runningFlag){
                _waitMutex.unlock();
                return 0;
            }
        }
    }
    _sortData[begin] = temp; //最后比较位填入坑内
    //qDebug()<<"then"<<_pitTemp<<_sortData;
    _waitMutex.unlock();
    emit sortUpdated();
    return begin;  //返回pos位置
}

#if 0
//挖坑填数
int AdjustArray(int s[], int l, int r) //返回调整后基准数的位置
{
    int i = l, j = r;
    int x = s[l]; //s[l]即s[i]就是第一个坑
    while (i < j)
    {
        // 从右向左找小于x的数来填s[i]
        while(i < j && s[j] >= x)
            j--;
        if(i < j)
        {
            s[i] = s[j]; //将s[j]填到s[i]中，s[j]就形成了一个新的坑
            i++;
        }

        // 从左向右找大于或等于x的数来填s[j]
        while(i < j && s[i] < x)
            i++;
        if(i < j)
        {
            s[j] = s[i]; //将s[i]填到s[j]中，s[i]就形成了一个新的坑
            j--;
        }
    }
    //退出时，i等于j。将x填到这个坑中。
    s[i] = x;

    return i;
}
//分治
void quick_sort1(int s[], int l, int r)
{
    if (l < r)
    {
        int i = AdjustArray(s, l, r);//先成挖坑填数法调整s[]
        quick_sort1(s, l, i - 1); // 递归调用
        quick_sort1(s, i + 1, r);
    }
}
#end if
