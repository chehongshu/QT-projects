# ifndef MAINWINDOW_H
# define MAINWINDOW_H

# include <QMainWindow>

//包含下面这3个头文件
# include <QtSerialPort/QSerialPort>
# include <QtSerialPort/QSerialPortInfo>
# include <QTimer>
# include <QString>
# include <QDebug>
# include <QTextStream>
# include <QLabel>
# include <QPixmap>
# include <QPixmapCache>
# include <psshowslider.h>
# include <Qextserial/qextserialport.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void handle_receive(QByteArray handle_Data);
private slots:
    void on_openCom_clicked();
    void on_openCom2_clicked();
    void readMyCom();
    void readMyCom2();
    void send_data_slot(int flag);
    void change_port();
private:
    Ui::MainWindow *ui;
    //添加这3个变量
    QextSerialPort  *my_serialPort;//(实例化一个指向串口的指针，可以用于访问串口)
    QextSerialPort *second_serialPort;
    QByteArray requestData;//（用于存储从串口那读取的数据）
    QByteArray requestData_another;
    QByteArray sendData;
    // 调试使接收到的数据完整的定义的QByteArray
    QByteArray data_first;
    QByteArray data_second;
    QByteArray data_third;
    QByteArray data_forth;
    QByteArray byteArray;

    QByteArray data_first_another;
    QByteArray data_second_another;
    QByteArray data_third_another;
    QByteArray data_forth_another;
    QByteArray byteArray_another;

    QTimer *timer;//（用于计时）
    QTimer *timer2;//（用于计时）
    QTimer *timer3;//（用于计时）
    QTimer *psstimer;
    QPixmap picture;
    QPixmap picture2;
    QPixmap run_pattern_pic;
    QPixmap jianxian_pic;
    QPixmap shangxiatingzhen_pic;
    QPixmap danchufa_pic;
    QPixmap ruanqidong_pic;
    QPixmap taiyajiao_pic;
    QPixmap logo;
    QPixmap  qiangufeng_pic;
    QPixmap  hougufeng_pic;
public:
    int changeflag;
    int alarm_flag;
    uchar sum=0x00;

};

#endif // MAINWINDOW_H
