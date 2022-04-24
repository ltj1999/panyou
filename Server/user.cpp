#include "user.h"

User::User(std::string u , std::string p , std::string n ,std::string m ,std::string b, TcpSocket *s)
    :_uid(u)
    ,_passwd(p)
    ,_userName(n)
    ,_sex(m)
    ,_birth(b)
    ,_s(s)
{

}


TcpSocket *User::getSocket()
{
    return _s;
}

const char *User::getName()
{
    return _userName.c_str();
}

const char* User::getUid()
{
    return _uid.c_str();
}

const char* User::getsex()
{
    return _sex.c_str();
}


const char *User::getbirth()
{
    return _birth.c_str();
}

