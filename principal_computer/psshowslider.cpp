#include "psshowslider.h"

psshowslider::psshowslider(QWidget *parent):QFrame(parent)
{

}
psshowslider::~psshowslider()
{
}

double psshowslider::getMaxValue()
{
    return maxvalue;
}
double psshowslider::getMinValue()
{
    return minvalue;
}
double psshowslider::getValue()
{
    return value;
}
//设置值
void psshowslider::setValue(double value)
{
    this->value=value;
}
//  重写函数，repaint和update函数可以调用
void psshowslider::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    //移动坐标系(很重要 可以控制控件在哪个地方)
    painter.translate(20,70);
    //设置画笔的类型等
    QPen pen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);//  相当于为画画的添加画笔
    painter.drawLine(0, 60, 0, -60);//画最左面的直线
    //画上边直线
    painter.drawLine(0, -60, 320, -60);
    //画下边直线
    painter.drawLine(0, 60, 320, 60);
    //画上下直线刻度以及文字
    //  画右面的直线
    painter.drawLine(270, 60, 270, -60);

    int num = 1;
    int x, y;
    //  第二个画笔
    QPen pen2(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPoint point;
    painter.setPen(pen2);
    // 设置 标签的 大小
    for (; num < 21; num++) {
        x = 15 * num;
        if (num % 2 == 0) {
            y = -50;
            point.setX(x - 5);
            point.setY(y + 10);
            painter.drawText(point, QString::number(225 * num));
        } else {
            y = -50;
        }
        painter.drawLine(x, -60, x, y);
    }

    //设置线性渐变
    QLinearGradient linearGradient = QLinearGradient(0, 0, 300, 0);
    linearGradient.setColorAt(0.0, Qt::cyan);// 第一个颜色
    linearGradient.setColorAt(0.6, Qt::yellow);// 第二个颜色
    linearGradient.setColorAt(1, Qt::red);// 第三个颜色
    //设置画刷
    QBrush qsh(linearGradient);
    painter.setBrush(qsh);
    painter.setPen(Qt::red);
    //画矩形
    ////   200/10=20
    painter.drawRect(0,-60, (int)(value/15.0), 120);//135==8000

}
