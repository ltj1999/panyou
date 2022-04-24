#ifndef USER_H
#define USER_H

#include "tcpsocket.h"
#include <string>

class User
{
public:
    User(std::string u , std::string p , std::string n ,std::string m ,std::string b, TcpSocket *s);
    TcpSocket *getSocket();

    const char *getName();

    const char *getUid();

    const char *getsex();

    const char *getbirth();

//    int getRank();

//    void setRank(int rank);

private:
//    std::string _uid;
    std::string _uid;               //用户uid
    std::string _passwd;    //用户密码
    std::string _userName;  //用户名
    std::string _sex;       //用户性别
    std::string _birth;     //用户出生日期

//    int _rank;              //rank分数

    TcpSocket *_s;          //通信套接字类

};

#endif // USER_H
