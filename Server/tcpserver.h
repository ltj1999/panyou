#ifndef TCPSERVER_H
#define TCPSERVER_H

#include"thread.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include "tcpsocket.h"

//class TcpSocket;

//Tcp服务器的基类
class TcpServer
{
    friend class TcpSocket;

public:
    TcpServer(int threadNum = 8);

    int listen(int port, const char* ip = NULL);

    //服务器开启
    void start();



protected:
    //监听回调函数，有客户端链接的时候会调用这个函数
    static void listenCb(struct evconnlistener *, evutil_socket_t, struct sockaddr *,int socklen , void *);

    //监听处理函数
    void listenEvent(evutil_socket_t fd,struct sockaddr_in *);

    //----------虚函数 去具体处理客户端逻辑---------//
    //客户端链接事件
    virtual void connectEvent(TcpSocket *){}
    //客户端可读
    virtual void readEvent(TcpSocket *){}
    //客户端可写
    virtual void writeEvent(TcpSocket *){}
    //关闭事件
    virtual void closeEvent(TcpSocket * , short ){}

private:
    int m_threadNum;      //线程个数
    Thread *m_threadPool; //线程池

    struct event_base *m_base;
    struct evconnlistener *m_listener; //监听客户端的链接

    int m_nextThread; //记录下一个线程的下标

};

#endif // TCPSERVER_H

