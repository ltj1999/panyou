#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "D:/application/ubuntu/ShareFolder/test-qt/common.h"

class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject *parent = nullptr);

    //向服务器发送消息
    void writeData(const QJsonObject &json);

signals:
    void rstResult (QJsonObject json);
    void testResult(QJsonObject json);
    void login(QJsonObject json);



//    //单人训练从服务器接受到题目
//    void receiveSingleQuestion(QJsonObject json);

//    void Rank(QJsonObject json);

public slots:
    //接受服务器的消息的信号槽
    void readData();

private:
    QTcpSocket s;

};

#endif // COMMUNICATE_H
