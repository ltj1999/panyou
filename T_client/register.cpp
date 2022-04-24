#include "register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <stdio.h>
#include <login.h>

Register::Register(Communicate *com, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    _com = com;



    connect(_com,SIGNAL(rstResult(QJsonObject)), this , SLOT(rstResult(QJsonObject)));
}

Register::~Register()
{
    delete ui;
}



void Register::rstResult(QJsonObject json)
{
    int result = json["result"].toInt();
    QString uid = json["uid"].toString();
    switch (result)
    {
    case OK:

        QMessageBox::information(this,"您的uid是",uid);
        break;

    case ERROR:
        QMessageBox::critical(this,"注册","未知错误");
        break;

    case USEREXIST:
        QMessageBox::critical(this,"注册","注册失败，用户uid已存在");
        break;


    }
}



void Register::on_back_pushButton_clicked()
{
    close();
}


//立即注册
void Register::on_register_pushButton_clicked()
{
    QString userName = ui->name_lineEdit->text();
    QString passwd = ui->passwd_lineEdit->text();
    QDate date = ui->dateEdit->date();

    QString year=QString::number(date.year());         //年
    QString month=QString::number(date.month());      //月
    QString day=QString::number(date.day());         //日


    //发送注册信息
    QJsonObject json;
    json["cmd"] = REGISTER;
    json["userName"] = userName;
    json["passwd"] = passwd;
    json["sex"] = sex;
    json["year"] = year;
    json["month"] = month;
    json["day"] = day;

    //给服务器发送数据
    _com->writeData(json);
}


void Register::on_male_pushButton_clicked()
{
    sex = "male";
   // QMessageBox::information(this,"选择",sex);
}

void Register::on_female_pushButton_clicked()
{
    sex = "female";
  //  QMessageBox::information(this,"选择",sex);
}

//int转换为string
std::string Register::to_string(int n)
{
    int m = n;
        char s[100];
        char ss[100];
        int i=0,j=0;
        if (n < 0)// 处理负数
        {
            m = 0 - m;
            j = 1;
            ss[0] = '-';
        }
        while (m>0)
        {
            s[i++] = m % 10 + '0';
            m /= 10;
        }
        s[i] = '\0';
        i = i - 1;
        while (i >= 0)
        {
            ss[j++] = s[i--];
        }
        ss[j] = '\0';
        return ss;
}




