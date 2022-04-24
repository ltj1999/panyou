#include "communicate.h"
#include <QHostAddress>
#include <QMessageBox>

Communicate::Communicate(QObject *parent) : QObject(parent)
{
    //虚拟机地址（服务器地址）192.168.226.130
    //链接服务器
   s.connectToHost(QHostAddress("192.168.226.130"),9999);
   //s.connectToHost(QHostAddress("192.168.142.135"),9999);

   //信号槽实现接受服务器消息
    connect(&s, SIGNAL(readyRead()),this,SLOT(readData()));


}

//接受服务器的消息
 void Communicate::readData()
 {
     QByteArray data;
     while(s.bytesAvailable())//判断有没有读完
     {
         data += s.readAll();
     }
     //数据解析
     QJsonDocument dt = QJsonDocument::fromJson(data);
     if(dt.isNull())
        return;

     QJsonObject root = dt.object();

     //具体的逻辑处理
     int cmd = root["cmd"].toInt();


     switch (cmd)
     {
     case REGISTER:

         emit rstResult(root);
         break;

     case LOGIN:

         emit login(root);
         break;

     default:
         break;
     }


 }

 //向服务器发送消息
 void Communicate::writeData(const QJsonObject &json)
 {
     QJsonDocument d(json);
     QByteArray sendData = d.toJson();
    int len =sendData.size();

   //向服务器发送两次数据，第一次数据长度，第二次数据
    s.write((char*)&len,sizeof(int)); //发送数据长度
    s.write(sendData);                //发送数据

 }
