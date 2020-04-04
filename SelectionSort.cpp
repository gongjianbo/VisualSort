#include "SelectionSort.h"

#include <QDebug>

SelectionSort::SelectionSort(QObject *parent)
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
        if(_iTemp<_sortData.count()&&_minTemp<_sortData.count())
            std::swap(_sortData[_iTemp],_sortData[_minTemp]);
        _animationProgress=0.0;
        _iTemp=_i;
        _minTemp=_min;
        _sortTimer.start();
        emit sortUpdated();
    });
}

void SelectionSort::runStart(int interval)
{
    _isFinish=false;
    _sortTimer.start(interval);
    //_animationLine.setDuration(interval);
}

/*
template<typename T>
void selection_sort(std::vector<T>& arr) {
        for (int i = 0; i < arr.size() - 1; i++) {
                int min = i;
                for (int j = i + 1; j < arr.size(); j++)
                        if (arr[j] < arr[min])
                                min = j;
                std::swap(arr[i], arr[min]);
        }
}
*/
void SelectionSort::runStep()
{
    //冒泡排序，两层循环，这里for替换为if，方便重入
    if(_i < _sortData.count()-1)
    {
        if ( _j < _sortData.count())
        {
            if(_sortData[_min]>_sortData[_j])
            {
                _min=_j; //下标的改变暂时不做动画
                _minTemp=_min;
            }
            _jTemp=_j++;
            return;
        }
        //交换动画初始化
        _iTemp=_i;
        _minTemp=_min;
        _sortTimer.stop();
        _animationProgress=0;
        //根据距离设置动画时间
        _animationLine.setDuration(((_minTemp-_iTemp)*5.0/_sortData.count()+1)*_sortTimer.interval());
        _animationLine.start();

        //std::swap(_sortData[_i],_sortData[_min]);
        ++_i;
        _j=_i+1;
        _jTemp=_i;
        _min=_i;
        return;
    }
    _isFinish=true;
    return;
}

bool SelectionSort::isFinish() const
{
    return _isFinish;
}

void SelectionSort::paint(QPainter *painter, int width, int height)
{
    //边框距离
    const int left_space=0;
    const int right_space=0;
    const int top_space=20;
    const int bottom_space=100;

    const int item_space=10;//横项间隔
    const int text_height=painter->fontMetrics().height();
    const int text_space=15;
    const double item_width=(width+item_space-left_space-right_space)/(double)_sortData.count()-item_space;
    const double item_bottom=height-bottom_space;
    const double height_factor=(height-top_space-bottom_space)/(double)_sortData.count();
    double item_left=0;
    double item_height=0;

    //qDebug()<<_iTemp<<_jTemp<<_minTemp<<_animationProgress;
    if(!_isFinish&&_minTemp<_sortData.count()&&
            _iTemp<_sortData.count()&&_jTemp<_sortData.count()){
        //min 和 第一层循环的 i 交换
        int i_idx=_iTemp;
        int j_idx=_jTemp;
        int m_idx=_minTemp;

        for(int i=0;i<_sortData.count();i++)
        {
            if(i==m_idx||i==i_idx||i==j_idx)
                continue;
            item_left=left_space+i*(item_width+item_space);
            item_height=height_factor*_sortData.at(i);
            painter->fillRect(item_left,item_bottom-item_height,
                              item_width,item_height,
                              QColor(200,200,200));
        }

        const double item_left_m=left_space+m_idx*(item_width+item_space);
        const double item_left_i=left_space+i_idx*(item_width+item_space);
        const double item_left_j=left_space+j_idx*(item_width+item_space);
        const double item_height_m=height_factor*_sortData.at(m_idx);
        const double item_height_i=height_factor*_sortData.at(i_idx);
        const double item_height_j=height_factor*_sortData.at(j_idx);

        const double item_offset=(item_left_m-item_left_i)*_animationProgress;
        //qDebug()<<item_offset<<(item_left_m-item_left_i)<<_animationProgress;

        //没有处理文字叠加情况
        // min
        painter->fillRect(item_left_m-item_offset,
                          item_bottom-item_height_m,
                          item_width,
                          item_height_m,
                          QColor(250,170,0));
        const QString item_text_m=QString::number(_sortData.at(m_idx));
        painter->drawText(item_left_m-item_offset
                          +(item_width-painter->fontMetrics().width(item_text_m))/2,
                          item_bottom+text_height+text_space,
                          item_text_m);

        // for i
        painter->fillRect(item_left_i+item_offset,
                          item_bottom-item_height_i,
                          item_width,
                          item_height_i,
                          QColor(0,170,250));
        const QString item_text_i=QString::number(_sortData.at(i_idx));
        painter->drawText(item_left_i+item_offset
                          +(item_width-painter->fontMetrics().width(item_text_i))/2,
                          item_bottom+text_height+text_space,
                          item_text_i);

        //这里有个堆叠顺序的问题，如果是交换状态，j应该置于底层
        // for j
        if(QTimeLine::Running==_animationLine.state()&&(j_idx==i_idx||j_idx==m_idx)){

        }else{
            painter->fillRect(item_left_j,
                              item_bottom-item_height_j,
                              item_width,
                              item_height_j,
                              QColor(0,200,0));
            const QString item_text_j=QString::number(_sortData.at(j_idx));
            painter->drawText(item_left_j
                              +(item_width-painter->fontMetrics().width(item_text_j))/2,
                              item_bottom+text_height+text_space,
                              item_text_j);
        }

        //记录min的下标值
        const double min_top=item_bottom+text_height+text_space*2;
        QPainterPath m_path;
        m_path.moveTo(item_left_m+item_width/2.0,min_top);
        m_path.lineTo(item_left_m,min_top+item_width/2.0);
        m_path.lineTo(item_left_m+item_width,min_top+item_width/2.0);
        m_path.lineTo(item_left_m+item_width/2.0,min_top);
        painter->fillPath(m_path,QColor(250,170,0));
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
