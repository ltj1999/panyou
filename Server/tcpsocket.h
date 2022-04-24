#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <sys/socket.h>
//#include "tcpserver.h"
#include <string>
class TcpServer;

//通信类，负责与客户端进行通信
class TcpSocket
{
        friend class TcpServer;

public:
    TcpSocket(TcpServer* server, struct bufferevent *bev, char *ip, u_int16_t port);

    //可读事件回调函数
    static void readEventCb(struct bufferevent *bev, void *ctx);
    //可写事件回调函数
    static void writeEventCb(struct bufferevent *bev, void *ctx);
    //异常事件回调函数
    static void closeEventCb(struct bufferevent *bev, short what ,void *ctx);

    char *getIp();          //获取IP地址
    u_int16_t getPort();    //获取端口

    //从客户端中读数据
    int readData(void *data , int size);

    //往客户端写数据
    int writeData(const void *data, int size);

    void setUserName(std::string name);
    std::string getUserName();


    void setUserId(std::string uid);
    std::string getUserId();

    void setUserSex(std::string sex);
    std::string getUserSex();

    void setUserBirth(std::string birth);
    std::string getUserBirth();


private:
    static TcpServer *m_tcpServer; //服务器类对象
    struct bufferevent *m_bev; //与客户端通信的句柄
    char *m_ip;             //客户端的IP地址
    u_int16_t m_port;       //客户端的使用端口

    std::string _uid;
    std::string _userName;        //当前用户
    std::string _sex;
    std::string _birth;
};

#endif // TCPSOCKET_H
