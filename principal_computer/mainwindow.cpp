#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/qtextcodec.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMinimumSize(1200, 700);
    setMaximumSize(1200, 700);
    //串口初始化
    my_serialPort = NULL;
    second_serialPort = NULL;

       QPalette palette= this->palette();//  创建调色板
       palette.setColor(QPalette::Window,QColor(255,255,255,255));// 设置整个背景为白色
       this->setPalette(palette);
       // logo图片加载
       logo.load("images/logo.png");ui->logo_label->setPixmap(logo);
      //在comboBox那添加串口号
      ui->comboBox_portName_first->addItem("COM1");ui->comboBox_portName_first->addItem("COM2");ui->comboBox_portName_first->addItem("COM3");
      ui->comboBox_portName_first->addItem("COM4");ui->comboBox_portName_first->addItem("COM5");ui->comboBox_portName_first->addItem("COM6");
      ui->comboBox_portName_first->addItem("COM7");ui->comboBox_portName_first->addItem("COM8");ui->comboBox_portName_first->addItem("COM9");
      ui->comboBox_portName_first->addItem("COM10");ui->comboBox_portName_first->addItem("COM11");ui->comboBox_portName_first->addItem("COM12");
      ui->comboBox_portName_second->addItem("COM1");ui->comboBox_portName_second->addItem("COM2");ui->comboBox_portName_second->addItem("COM3");
      ui->comboBox_portName_second->addItem("COM4");ui->comboBox_portName_second->addItem("COM5");ui->comboBox_portName_second->addItem("COM6");
      ui->comboBox_portName_second->addItem("COM7");ui->comboBox_portName_second->addItem("COM8");ui->comboBox_portName_second->addItem("COM9");
      ui->comboBox_portName_second->addItem("COM10");ui->comboBox_portName_second->addItem("COM11");ui->comboBox_portName_second->addItem("COM12");



     setWindowTitle("泰来");
     //  初始化
     requestData.resize(20);
     requestData_another.resize(20);

     QPixmapCache::setCacheLimit(1);

     QObject::connect(ui->change_Button,SIGNAL(clicked(bool)),this,SLOT(change_port()));
     QObject::connect(ui->openCom,SIGNAL(clicked(bool)),this,SLOT(on_openCom_clicked()));
     QObject::connect(ui->openCom2,SIGNAL(clicked(bool)),this,SLOT(on_openCom2_clicked()));


     changeflag = 0;
     alarm_flag = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//打开串口按钮按下  配置串口参数
void MainWindow::on_openCom_clicked()
{
    if( my_serialPort != NULL)
    {
        delete my_serialPort;
    }
        QString portname = ui->comboBox_portName_first->currentText();
        my_serialPort = new QextSerialPort(portname);
        // 波特率
        my_serialPort->setBaudRate(BAUD38400);
        // 数据位
        my_serialPort->setDataBits(DATA_8);
        // 设置校验位
        my_serialPort->setParity(PAR_NONE);
        // 设置停止位
        my_serialPort->setStopBits(STOP_1);
        // 设置数据流
        my_serialPort->setFlowControl(FLOW_OFF);
        my_serialPort->setTimeout(200);

        bool flag = my_serialPort->open(QIODevice::ReadWrite);


            connect(my_serialPort,SIGNAL(readyRead()),this,SLOT(readMyCom()));


        if(false == flag)
        {
            QMessageBox::warning(this,tr("Warning"),tr("串口打开失败或端口已被打开"));
        }else if(true ==flag)
        {
            QMessageBox::information(this,tr("information"),tr("串口打开成功"));
        }

}
void MainWindow::on_openCom2_clicked()
{

        if(second_serialPort != NULL)
        {
            delete second_serialPort;
        }
        QString portname2 = ui->comboBox_portName_second->currentText();
        second_serialPort = new QextSerialPort(portname2);
        //串口数据读取连接
        connect(second_serialPort, SIGNAL(readyRead()), this, SLOT(readMyCom2()) );
        //信号与槽函数

        // 波特率
        second_serialPort->setBaudRate(BAUD38400);
        // 数据位
        second_serialPort->setDataBits(DATA_8);
        // 设置校验位
        second_serialPort->setParity(PAR_NONE);
        // 设置停止位
        second_serialPort->setStopBits(STOP_1);
        // 设置数据流
        second_serialPort->setFlowControl(FLOW_OFF);
        second_serialPort->setTimeout(200);

        bool flag2 = second_serialPort->open(QIODevice::ReadWrite);

        if(false == flag2)
        {
            QMessageBox::warning(this,tr("Warning"),tr("串口打开失败或端口已被打开"));
        } else if(true == flag2)
        {
            QMessageBox::information(this,tr("information"),tr("串口打开成功"));
        }

}

 //读取缓冲的数据，每秒读一次
void MainWindow::readMyCom()
{
        if(changeflag==0)
        {
            qDebug() << "我能读到  哈哈哈哈哈第一个";

             static int i = 0;
             static int k = 0;
             int flag = 0;
             byteArray = NULL;
             i++;
             if(i==1)
             {
                   k=0;
                   data_first = my_serialPort->readAll();//用requestData存储从串口那读取的数据  读取第一个串口数据
                   qDebug() << "data_first  :"  << data_first.toHex();

             }
             if(i==2)
             {
                   data_second = my_serialPort->readAll();//用requestData存储从串口那读取的数据  读取第一个串口数据
                   qDebug() << "data_second  :"  << data_second.toHex();
             }
             if(i==3)
             {
                    data_third = my_serialPort->readAll();
             }
             if(i==4)
             {

                    data_forth = my_serialPort->readAll();
                    data_first.append(data_second);
                    data_first.append(data_third);
                    data_first.append(data_forth);
                    requestData = data_first;
                    i=0;
                    //  对四次发来的数据进行一个细致的处理
                    for(int j=0;j<requestData.length();j++)
                    {
                        if((requestData.at(j)==0x55)||flag)
                        {
                            flag=1;
                            byteArray.append(requestData.at(j));
                            k++;
                            if(k==14)
                            {
                                break;
                            }
                        }
                    }
                 }

               requestData=byteArray;
               handle_receive(requestData);
               qDebug() << "byteArray  :"  << byteArray.toHex();
               qDebug() << "requestData  :"  << requestData.toHex();
               requestData.clear();    //清除缓冲区
               byteArray.clear();
        }
}

void MainWindow::readMyCom2()
{
    if(changeflag!=0)
    {
        qDebug() << "我能读到  哈哈哈哈哈第二个";
    static int i_another = 0;
    static int k_another = 0;
    int flag_another = 0;
    byteArray_another = NULL;
    i_another++;
    if(i_another==1)
    {
          k_another=0;
          data_first_another = second_serialPort->readAll();//用requestData存储从串口那读取的数据  读取第一个串口数据
          qDebug() << "data_first_another  :"  << data_first_another.toHex();

    }
    if(i_another==2)
    {
          data_second_another = second_serialPort->readAll();//用requestData存储从串口那读取的数据  读取第一个串口数据
          qDebug() << "data_second_another  :"  << data_second_another.toHex();
    }
    if(i_another==3)
    {
       data_third_another = second_serialPort->readAll();
       qDebug() << "data_third_another  :"  << data_third_another.toHex();
    }

    if(i_another==4)
    {

       data_forth_another = second_serialPort->readAll();
       qDebug() << "data_forth_another  :"  << data_forth_another.toHex();

       data_first_another.append(data_second_another);
       data_first_another.append(data_third_another);
       data_first_another.append(data_forth_another);
       requestData_another = data_first_another;
       qDebug() << "   第四个结合之后  :"  << requestData_another.toHex();
       i_another=0;
       for(int j_another=0;j_another<requestData_another.length();j_another++)
       {
           if((requestData_another.at(j_another)==0x55)||flag_another)
           {
               flag_another=1;
               byteArray_another.append(requestData_another.at(j_another));
               k_another++;
               if(k_another==14)
               {
                   break;
               }
           }
       }
    }

    requestData_another=byteArray_another;
    qDebug() << "byteArray_another  :"  << byteArray_another.toHex();
    qDebug() << "requestData_another  :"  << requestData_another.toHex();
    handle_receive(requestData_another);



      //  处理  第一个 串口
      //  以16进制的形式显示
      //ui->textBrowser->setText(requestData.toHex());
      //data_debug.clear();
      requestData_another.clear();    //清除缓冲区
       byteArray_another.clear();
       }
}

//  写数据
void MainWindow::send_data_slot(int changeflag)
{ 
    if(changeflag==0)
    {
             if(my_serialPort->open(QIODevice::ReadWrite))
             {
                my_serialPort->write(sendData);
             }
    }else{
            if(second_serialPort->open(QIODevice::ReadWrite))
            {
                second_serialPort->write(sendData);
            }
    }

}

// 改变 串口种类。
void MainWindow::change_port()
{
     changeflag =~ changeflag;
       qDebug() << "changeflag  :"  << changeflag;//   -1   和    0
}

void MainWindow::handle_receive(QByteArray handle_Data)
{
        sum=0;// qingling
        qDebug()<<"进来了";
    //QByteArray qba = QByteArray::fromHex(handle_Data.toHex());   //十六进制需用fromHex转成普通字符
     if(handle_Data.at(0)==0x55) //  接受起始位
     {
        qDebug()<<"进来了第二道";
         for(int i=0;i<=12;i++)
         {
             sum+=(unsigned char)handle_Data.at(i);
         }
           qDebug()<<"sum="<<sum;
           qDebug()<<"handle_Data"<<handle_Data.toHex();
           qDebug()<<"13geshu"<<(unsigned char)handle_Data.at(13);
         //  校验位ok
         if((sum&0xff)==((unsigned char)handle_Data.at(13)))
         {


             qDebug()<<"进来了第三道";
              sendData[0] = 0x55 ;
              sendData[1] = 0x55 ;
              sendData[2] = 0x00 ;    //  表示接受正常
              sendData[3] = 0xAA ;
              send_data_slot(changeflag);

              //缝纫模式
              //1：自由缝 2：定长缝 3：重逢   4：多段缝  显示在一个区域
              //  Byte1    有问题
              switch((((unsigned char)handle_Data.at(1))&0x0f))
              {

                 case 0x01:  QPixmapCache::clear();picture.load("images/ziyoufeng.jpg"); ui->fengren_picture->setPixmap(picture);
                 ui->fengrenlabel->setText(" 自由缝");break;//0：自由缝    //
                 case 0x02:  QPixmapCache::clear();picture.load("images/dingchangfeng.jpg"); ui->fengren_picture->setPixmap(picture);
                 ui->fengrenlabel->setText(" 定长缝");break;//1：定长缝
                 case 0x04:   QPixmapCache::clear();picture.load("images/chongfeng.jpg"); ui->fengren_picture->setPixmap(picture);
                 ui->fengrenlabel->setText("   重逢"); break;//2：重逢
                 case 0x08:   QPixmapCache::clear();picture.load("images/duoduanfeng.jpg"); ui->fengren_picture->setPixmap(picture);
                 ui->fengrenlabel->setText(" 多段缝"); break;//3：多段缝
                 default :QPixmapCache::clear();picture.load("images/error.png"); ui->fengren_picture->setPixmap(picture);
                      ui->fengrenlabel->setText("输入error"); break;
               }
              //Byte2
                     //帧A
                      int A_data= (unsigned char)handle_Data.at(2);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                      QString A = QString::number(A_data, 10);  // int16进制转换为10进制的String 的数。
                      ui->A_number->setText(A);
              //Byte3          //帧B
                      int B_data= (unsigned char)handle_Data.at(3);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                      QString B = QString::number(B_data, 10);  // int16进制转换为10进制的String 的数。
                      ui->B_number->setText(B);
              //Byte4         //帧C
                      int C_data= (unsigned char)handle_Data.at(4);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                      QString C = QString::number(C_data, 10);  // int16进制转换为10进制的String 的数。
                      ui->C_number->setText(C);
              //Byte5       //帧D
                      int D_data= (unsigned char)handle_Data.at(5);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                      QString D = QString::number(D_data, 10);  // int16进制转换为10进制的String 的数。
                      ui->D_number->setText(D);
                 //  前固缝，后固封， 上/下停针图片没有  //  未初始化


                //speed   高八位乘以256+第八位转换为16进制
                //  byte6为高 8 位
                   int speed1_data= (unsigned char)handle_Data.at(6);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                //   QString speed1 = QString::number(speed1_data, 10);  // int16进制转换为10进制的String 的数。
                //  byte7为低 8 位
                   int speed2_data= (unsigned char)handle_Data.at(7);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                //   QString  speed2 = QString::number(speed2_data, 10);  // int16进制转换为10进制的String 的数。

                   int speed_sum = speed1_data<<8|speed2_data;//移位8位是一个字节
                   QString  speed = QString::number(speed_sum,10);

                //  Byte6 先往左移 4位之后和Byte7 或以下
                //  speed = speed1.append(speed2);
                      ui->speed_view->setText(speed);

                //  Byte8    自动化   剪线   上下停针  单触发   软启动  抬压脚
                //  剪线  Byte8   bit4
                if(((unsigned char)handle_Data.at(8)&0x10)==0x10)
                 {
                     QPixmapCache::clear();jianxian_pic.load("images/jiandao.jpg"); ui->jianxian_label->setPixmap(jianxian_pic);
                 }else{
                     QPixmapCache::clear();jianxian_pic.load("images/kongbai.png"); ui->jianxian_label->setPixmap(jianxian_pic);
                 }
                // 上下停针   1为上针  0为  上针
                if(((unsigned char)handle_Data.at(8)&0x20)==0x20)
                 {
                     QPixmapCache::clear();shangxiatingzhen_pic.load("images/shangtingzhen.jpg"); ui->shangxiatingzhen_label->setPixmap(shangxiatingzhen_pic);
                     ui->tingzhen_label->setText(" 上停针");
                 }else{
                     QPixmapCache::clear();shangxiatingzhen_pic.load("images/xiatingzhen.jpg"); ui->shangxiatingzhen_label->setPixmap(shangxiatingzhen_pic);
                       ui->tingzhen_label->setText(" 下停针");
                }
                //  单触发
                if(((unsigned char)handle_Data.at(8)&0x40)==0x40)
                 {
                     QPixmapCache::clear();danchufa_pic.load("images/danchufa.jpg"); ui->danchufa_label->setPixmap(danchufa_pic);
                 }else{
                     QPixmapCache::clear();danchufa_pic.load("images/kongbai.png"); ui->danchufa_label->setPixmap(danchufa_pic);
                }
               //  软启动  Byte8
                if(((unsigned char)handle_Data.at(8)&0x80)==0x80)
                {
                     QPixmapCache::clear();ruanqidong_pic.load("images/ruanqidong.jpg"); ui->ruanqidong_label->setPixmap(ruanqidong_pic);
                }else{
                     QPixmapCache::clear();ruanqidong_pic.load("images/kongbai.png"); ui->ruanqidong_label->setPixmap(ruanqidong_pic);
                }
                  //  抬压脚  Byte1 的第八位
                if(((unsigned char)handle_Data.at(1)&0x80)==0x80)
                {
                     QPixmapCache::clear();taiyajiao_pic.load("images/taiyajiao.jpg"); ui->taiyajiao_label->setPixmap(taiyajiao_pic);
                }else{
                     QPixmapCache::clear();taiyajiao_pic.load("images/kongbai.png"); ui->taiyajiao_label->setPixmap(taiyajiao_pic);
                }


                    // 前固缝
                if((handle_Data.at(8)&0x01)==0x01)
                {
                      QPixmapCache::clear();qiangufeng_pic.load("images/qianshuanggufeng.jpg");
                      ui->qiangufeng_label->setPixmap(qiangufeng_pic);
                }

                if((handle_Data.at(8)&0x04)==0x04)
                {
                      QPixmapCache::clear();qiangufeng_pic.load("images/qiansigufeng.jpg");
                      ui->qiangufeng_label->setPixmap(qiangufeng_pic);
                }
                if(((handle_Data.at(8)&0x01)!=0x01)&&((handle_Data.at(8)&0x04)!=0x04))
                {
                      QPixmapCache::clear();qiangufeng_pic.load("images/kongbai.jpg");
                      ui->qiangufeng_label->setPixmap(qiangufeng_pic);
                }
                // 后固缝
                if((handle_Data.at(8)&0x02)==0x02)
                {
                      QPixmapCache::clear();hougufeng_pic.load("images/hougushuangfeng.jpg");
                      ui->hougufeng_label->setPixmap(hougufeng_pic);
                }

                if((handle_Data.at(8)&0x08)==0x08)
                {
                      QPixmapCache::clear();hougufeng_pic.load("images/housishuangfeng.jpg");
                      ui->hougufeng_label->setPixmap(hougufeng_pic);
                }

                if(((handle_Data.at(8)&0x02)!=0x02)&&((handle_Data.at(8)&0x08)!=0x08))
                {
                      QPixmapCache::clear();hougufeng_pic.load("images/kongbai.jpg");
                      ui->hougufeng_label->setPixmap(hougufeng_pic);
                }
      //  Byte9  // ALARM
              switch((unsigned char)handle_Data.at(9))
              {
                   case 0x00:  alarm_flag = 1; break;
                   default:
                   QPixmapCache::clear();run_pattern_pic.load("images/Error.jpg");
                   ui->run_pattern_label->setPixmap(run_pattern_pic);
                   int ALARM_data= requestData.at(9);
                   QString ALARM_number = QString::number(ALARM_data, 10);
                   ui->alarm->setText(" Err"+ALARM_number);
               }
             if(alarm_flag==1)
             {
                 if(((unsigned char)handle_Data.at(1)&0x40)==0x40)
                 {
                     // 运转的
                     ui->alarm->setText("正常运行");
                     QPixmapCache::clear();run_pattern_pic.load("images/Run.jpg");
                     ui->run_pattern_label->setPixmap(run_pattern_pic);
                     alarm_flag=0;
                 }else{
                     //停止的
                     ui->alarm->setText(" 停机");
                     QPixmapCache::clear();run_pattern_pic.load("images/Stop.jpg");
                     ui->run_pattern_label->setPixmap(run_pattern_pic);
                     alarm_flag=0;
                 }

             }





               //  Byte10  剪刀 缝纫 计数器
                 unsigned char jiandaojishu= (unsigned char)handle_Data.at(10);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
                 QString jiandajishuqi = QString::number(jiandaojishu, 10);  // int16进制转换为10进制的String 的数。
                 ui->fengren_speed_view->setText(jiandajishuqi);




               //速度曲线的
               //  byte11为高 8 位
                int speed1_curve_data= (unsigned char)handle_Data.at(11);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
//              QString speed1_curve = QString::number(speed1_curve_data, 10);  // int16进制转换为10进制的String 的数。

                 //  byte12为低 8 位
                 int speed2_curve_data= (unsigned char)handle_Data.at(12);  // 取出 对应位的16进制char型之后转换为16进制的int型（强制转换）
    //             QString  speed2_curve = QString::number(speed2_curve_data, 10);  // int16进制转换为10进制的String 的数。
                 int speed_curve_sum = speed1_curve_data<<8|speed2_curve_data;
                 QString  speed_curve = QString::number(speed_curve_sum,10);
                 //speed_curve = speed1_curve.append(speed2_curve);// 显示的 速度曲线的大小。
                  qDebug() << "speed_curve  :"  << speed_curve;
                 ui->pss->setValue(speed_curve.toDouble());
                 ui->pss->repaint();


                 //  标志位清零
                 alarm_flag=0;



         }else {
              sendData[0] = 0x55 ;
              sendData[1] = 0x55 ;
              sendData[2] = 0x01 ;  //  表示接受错误
              sendData[3] = 0xAB ;
              send_data_slot(changeflag);
         }

   }


}

