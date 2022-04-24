#ifndef MYSERVER_H
#define MYSERVER_H
#include "tcpserver.h"
#include <spdlog/spdlog.h>
#include "db.h"
#include <json/json.h>
#include <json/reader.h>
#include "/mnt/hgfs/ShareFolder/test-qt/common.h"
#include <string>
#include <map>
#include "user.h"
#include <time.h>
#include<stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>


#define DEBUG

//int uid_num ;

class MyServer :public TcpServer
{
public:
    MyServer();
//    friend class Db;


    //客户端链接事件
    void connectEvent(TcpSocket *);
    //客户端可读
    void readEvent(TcpSocket *);
    //客户端可写
    void writeEvent(TcpSocket *);
    //关闭事件
    void closeEvent(TcpSocket * s, short );

private:

    //发送数据给客户端
    void writeData(TcpSocket *s,const Json::Value &inJson);

    //用户注册
    void Register(TcpSocket *s, const Json::Value &inJson);

    //用户登录
    void Login(TcpSocket *s, const Json::Value &inJson);

    //设置偷袭
    void SetPic(TcpSocket *s, const Json::Value &inJson);

    //随机获取uid
    std::string Setuid(int n);

    //转换int为string
    std::string to_string(int n);

    //检查uid是否重复
    int Checkuid(std::string c_uid);





private:
    std::shared_ptr<spdlog::logger> _log; //记录日志的句柄
    DB* _db; //数据库句柄

    //键是用户名，值是用户指针
    std::mutex _userLock;
    std::map<std::string , User*> _users;  //在线用户列表




};

#endif // MYSERVER_H
