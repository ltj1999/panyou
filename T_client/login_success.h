#ifndef LOGIN_SUCCESS_H
#define LOGIN_SUCCESS_H

#include <QDialog>
#include "communicate.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QFileInfo>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <QMessageBox>

using namespace std;

namespace Ui {
class login_success;
}

class login_success : public QDialog
{
    Q_OBJECT

public:
    explicit login_success(Communicate *com,QJsonObject &json,QWidget *parent = 0);
    ~login_success();

private slots:

    void on_set_pic_Button_clicked();

    void on_send_pic_Button_clicked();

private:

    //设置用户属性
    void setUserName();
    void setUserUid();
    void setUserSex();


private:
    Ui::login_success *ui;
    Communicate *_com;      //通信类指针， 负责和服务器通信

    QString _userName;       //自己名字
    QString _userUid;        //自己的uid
    QString _userSex;        //自己的性别
    QString _userBirth;     //生日
    QString OpenFile, OpenFilePath;
    QImage image;
    QString pic_path;
    char *pic_cpath;
};

#endif // LOGIN_SUCCESS_H
