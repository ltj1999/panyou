#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "communicate.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void login(QJsonObject json);

    void on_registerButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::Widget *ui;

    Communicate *_com;   //通信类指针， 负责和服务器通信
};

#endif // WIDGET_H
