#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QJsonObject>
#include "communicate.h"
#include "D:/application/ubuntu/ShareFolder/test-qt/common.h"

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(Communicate *com, QWidget *parent = 0);
    ~Register();

private slots:

    void rstResult(QJsonObject json);

    void on_back_pushButton_clicked();

    void on_register_pushButton_clicked();

    void on_male_pushButton_clicked();

    void on_female_pushButton_clicked();



private:
    Ui::Register *ui;

    QString sex = "male";

    Communicate *_com;

    std::string to_string(int n);
};

#endif // REGISTER_H
