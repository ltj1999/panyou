#include "login.h"
#include "ui_widget.h"
#include "register.h"
#include "QMessageBox"
#include "login_success.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    _com = new Communicate();
    connect(_com,SIGNAL(login(QJsonObject)), this , SLOT(login(QJsonObject)));

}

Widget::~Widget()
{
    delete ui;
}

//登录功能
void Widget::login(QJsonObject json)
{
    int result = json["result"].toInt();
    switch (result)
    {
    case OK:
    {

        this->hide();
        login_success *dlg = new login_success(_com,json);
        dlg->show();;
        dlg->setAttribute(Qt::WA_DeleteOnClose);

        break;
    }
    case ERROR:
        QMessageBox::critical(this,"登录","未知错误");
        break;

    case USERLOGIN:
        QMessageBox::critical(this,"登录","登录失败，不允许重复登录");
        break;

    case NAMEORPASSWD:
        QMessageBox::critical(this,"登录","用户名或密码错误");
        break;

    default:
        break;
    }
}

//注册按钮
void Widget::on_registerButton_clicked()
{
    this->hide();
    Register reg(_com);
    reg.exec();
    this->show();
}

//登录按钮
void Widget::on_loginButton_clicked()
{
    QString uid = ui->id_lineEdit->text();
    QString passwd = ui->passwd_lineEdit->text();

    //将登陆信息发送给服务器
    QJsonObject json;
    json["cmd"] = LOGIN;
    json["uid"] = uid;
    json["passwd"] = passwd;

    //给服务器发送数据
    _com->writeData(json);
}
