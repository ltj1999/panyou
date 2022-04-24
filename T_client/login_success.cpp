#include "login_success.h"
#include "ui_login_success.h"
#include <QLineEdit>
#include <QImage>
#include <QFileInfo>
#include <QFileDialog>

login_success::login_success(Communicate *com,QJsonObject &json,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_success)
{
    ui->setupUi(this);
    _com = com;

    _userName = json["name"].toString();
    _userUid = json["uid"].toString();
    _userSex = json["sex"].toString();
    _userBirth = json["birth"].toString();

    ui->name_label->setText(_userName);
    ui->uid_label->setText(_userUid);
    ui->sex_label->setText(_userSex);
    ui->birth_labek->setText(_userBirth);

}

login_success::~login_success()
{
    delete ui;
}



void login_success::on_set_pic_Button_clicked()
{
        OpenFile = QFileDialog::getOpenFileName(this,
            "please choose an image file",
            "",
            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
        if (OpenFile != "")
        {
            if (image.load(OpenFile))
            {
                //仅仅只是导入之后的图片仍然是原来的大小，这个时候我们需要缩放
                ui->pic_label->setPixmap(QPixmap::fromImage(image).scaled(ui->pic_label->size()));
            }

        }
        //显示所有的图片路径
        QFileInfo OpenFileInfo;
        OpenFileInfo = QFileInfo(OpenFile);
        OpenFilePath = OpenFileInfo.filePath();
        ui->lineEdit->setText(OpenFilePath);
        pic_path = OpenFilePath;

}

void login_success::on_send_pic_Button_clicked()
{
//     string buffer;
//     QString qbuffer;

     QJsonObject json;
     json["cmd"] = PIC;
     json["uid"] = _userUid;

    if(pic_path == NULL)
    {
        QMessageBox::critical(this,"错误","请选择一个图片");

    }
    else
    {
        //QString 形式转换为 char*
        QByteArray ba =  OpenFilePath.toLatin1();
        pic_cpath = ba.data();

       FILE *in = fopen(pic_cpath,"rb");
       if(in)
       {

       }




//       if(in)
//       {
//           //计算图片长度
//           fseek(in,0L,SEEK_END);
//           int length = ftell(in);
//           fseek(in,0L,SEEK_SET);
//           //创建内存缓冲区

//           char *buffer = new char[length];
//           fread(buffer,sizeof(char),length,in);



//           fclose(in);

//           QJsonObject json;
//           json["cmd"] = PIC;
//           json["uid"] = _userUid;
//           json["buffer"] = buffer;


//           _com->writeData(json);
//           delete []buffer;
//       }

    }

}
