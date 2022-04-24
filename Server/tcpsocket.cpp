#include "tcpsocket.h"

#include "tcpserver.h"

TcpServer *TcpSocket::m_tcpServer = NULL;

TcpSocket::TcpSocket(TcpServer* server, struct bufferevent *bev, char *ip, u_int16_t port)
{
    m_tcpServer = server;
    m_bev = bev;
    m_ip = ip;
    m_port = port;


}


//可读事件回调函数
void TcpSocket::readEventCb(struct bufferevent *, void *ctx)
{
    TcpSocket *s =(TcpSocket*)ctx;
    m_tcpServer->readEvent(s);
}
//可写事件回调函数
void TcpSocket::writeEventCb(struct bufferevent *, void *ctx)
{
    TcpSocket *s =(TcpSocket*)ctx;
    m_tcpServer->writeEvent(s);
}
//异常事件回调函数
void TcpSocket::closeEventCb(struct bufferevent *, short what ,void *ctx)
{
    TcpSocket *s =(TcpSocket*)ctx;
    m_tcpServer->closeEvent(s,what);

    delete  s;
}


char *TcpSocket::getIp()          //获取IP地址
{
    return m_ip;
}

u_int16_t TcpSocket::getPort()    //获取端口
{
    return m_port;
}

//从客户端中读数据
int TcpSocket::readData(void *data , int size)
{
    return bufferevent_read(m_bev,data , size);
}

//往客户端写数据
int TcpSocket::writeData(const void *data, int size)
{
    return bufferevent_write(m_bev , data , size);
}



void TcpSocket::setUserName(std::string name)
{
    _userName = name;
}


std::string TcpSocket::getUserName()
{
    return _userName;
}


void TcpSocket::setUserId(std::string uid)
{
    _uid = uid;
}
std::string TcpSocket::getUserId()
{
    return  _uid;
}

void TcpSocket::setUserSex(std::string sex)
{
    _sex = sex;
}
std::string TcpSocket::getUserSex()
{
    return  _sex;
}


void TcpSocket::setUserBirth(std::string birth)
{
    _birth = birth;
}
std::string TcpSocket::getUserBirth()
{
    return  _birth;
}

















