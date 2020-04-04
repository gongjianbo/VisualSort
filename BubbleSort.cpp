#include "BubbleSort.h"

#include <QDebug>

BubbleSort::BubbleSort(QObject *parent)
    : SortObejct(parent)
{
    //排序的定时器
    connect(&_sortTimer,&QTimer::timeout,this,[this]{
        if(isFinish()){
            _sortTimer.stop();
            emit sortFinished();
        }else{
            runStep();
        }
        emit sortUpdated();
    });

    _animationLine.setFrameRange(0,100);
    connect(&_animationLine,&QTimeLine::frameChanged,this,[this](int frame){
        _animationProgress=frame/100.0;
        emit sortUpdated();
    });
    connect(&_animationLine,&QTimeLine::finished,this,[this]{
        if(_swapIndex+1<_sortData.count())
            std::swap(_sortData[_swapIndex],_sortData[_swapIndex+1]);
        _swapIndex=0;
        _animationProgress=0;
        _sortTimer.start();
        emit sortUpdated();
    });
}

void BubbleSort::runStart(int interval)
{
    _isFinish=false;
    _sortTimer.start(interval);
    _animationLine.setDuration(interval);
}

void BubbleSort::runStep()
{
    //冒泡排序，两层循环，这里for替换为if，方便重入
    //本来想尝试QEventLoop，但是中途退出报异常
    if(_i < _sortData.count()-1)
    {
        if ( _j < _sortData.count()-1-_i)
        {
            if(_sortData[_j]>_sortData[_j+1])
            {
                //交换动画初始化
                _swapIndex=_j;
                _sortTimer.stop();
                _animationProgress=0;
                _animationLine.start();
            }
            _jTemp=_j++;
            return;
        }
        ++_i;
        _j=0;
        _jTemp=0;
        return;
    }
    _isFinish=true;
    return;
}

bool BubbleSort::isFinish() const
{
    return _isFinish;
}

void BubbleSort::paint(QPainter *painter, int width, int height)
{
    //边框距离
    const int left_space=0;
    const int right_space=0;
    const int top_space=20;
    const int bottom_space=50;

    const int item_space=10;//横项间隔
    const int text_height=painter->fontMetrics().height();
    const int text_space=15;
    const double item_width=(width+item_space-left_space-right_space)/(double)_sortData.count()-item_space;
    const double item_bottom=height-bottom_space;
    const double height_factor=(height-top_space-bottom_space)/(double)_sortData.count();
    double item_left=0;
    double item_height=0;

    if(!_isFinish&&(_jTemp<(_sortData.count()-1)||_swapIndex<(_sortData.count()-1))){
        //绘制待交换的数据条，绿色可交换，红色不可交换
        int idx=_jTemp;
        if(QTimeLine::Running==_animationLine.state()){
            idx=_swapIndex;
        }

        for(int i=0;i<_sortData.count();i++)
        {
            if(i==idx||i==idx+1)
                continue;
            item_left=left_space+i*(item_width+item_space);
            item_height=height_factor*_sortData.at(i);
            painter->fillRect(item_left,item_bottom-item_height,
                              item_width,item_height,
                              QColor(200,200,200));
        }

        const double item_left_1=left_space+idx*(item_width+item_space);
        const double item_left_2=left_space+(idx+1)*(item_width+item_space);
        const double item_height_1=height_factor*_sortData.at(idx);
        const double item_height_2=height_factor*_sortData.at(idx+1);
        const double animation_offset=(item_left_2-item_left_1)*_animationProgress;


        //第一个数据
        painter->fillRect(item_left_1+animation_offset,item_bottom-item_height_1,
                          item_width,item_height_1,
                          QColor(250,170,0));
        const QString left_text=QString::number(_sortData.at(idx));
        painter->drawText(item_left_1+animation_offset
                          +(item_width-painter->fontMetrics().width(left_text))/2,
                          item_bottom+text_height+text_space,
                          left_text);
        //第二个数据
        painter->fillRect(item_left_2-animation_offset,item_bottom-item_height_2,
                          item_width,item_height_2,
                          (_sortData.at(idx)<_sortData.at(idx+1)
                           ?QColor(200,0,0)
                          :QColor(0,200,0)));
        const QString right_text=QString::number(_sortData.at(idx+1));
        painter->drawText(item_left_2-animation_offset
                          +(item_width-painter->fontMetrics().width(right_text))/2,
                          item_bottom+text_height+text_space,
                          right_text);
    }else{
        //绘制所有的数据条
        for(int i=0;i<_sortData.count();i++)
        {
            item_left=left_space+i*(item_width+item_space);
            item_height=height_factor*_sortData.at(i);
            painter->fillRect(item_left,item_bottom-item_height,
                              item_width,item_height,
                              QColor(200,200,200));
        }
    }
}
