#ifndef PSSHOWSLIDER_H
#define PSSHOWSLIDER_H

#include "QPaintEvent"
#include "QFrame"
#include "QPainter"
#include "QColor"
#include "QDebug"
#include "QWidget"

class psshowslider : public QFrame
{
    Q_OBJECT

public:
   explicit psshowslider(QWidget * parent = 0);
    ~psshowslider();
    //设置最大值
    void setMaxValue(double maxvalue);
    //得到最大值
    double getMaxValue();

    //设置最小值
    void setMinValue(double minvalue);
    //得到最小值
    double getMinValue();

    //设置值
    void setValue(double value);

    //取得值
    double getValue();

   //重画函数
    void paintEvent(QPaintEvent *);

public:
    //滑动条最大值
    double maxvalue;
    //滑动条最小值
    double minvalue;
    //设置显示的值
    double value;

};
#endif // PSSHOWSLIDER_H
