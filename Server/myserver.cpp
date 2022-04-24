#include "myserver.h"





MyServer::MyServer()
{
#ifdef DEBUG
    _log =  spdlog::stdout_color_mt("TestQt");
#else
    _log = spdlog::rotating_logger_mt("TestQt","TestQt", 1024*1024*5, 3);
    _log->flush_on(spdlog::level::info);
#endif
    //链接数据库
    _db = new DB(NULL , "root" , "123456" , "TestQt");


}


//客户端链接事件
void MyServer::connectEvent(TcpSocket *s)
{
    _log->info("有一个新链接[{}:{}]",s->getIp() , s->getPort());
}
//客户端可读
void MyServer::readEvent(TcpSocket *s)
{
    char buf[1024] = {0};
    while(1)
    {
        int len = 0;
        //第一次读数据长度
        s->readData(&len,sizeof(len));
        if(len<=0)
            break;
        //第二次读数据本身
        s->readData(buf,len);

        //数据解析
        Json::Value root;
        Json::Reader reader; //Json解析器

        if(!reader.parse(buf,root))
        {
            _log->error("json 数据解析失败");
            return;
        }
        int cmd = root["cmd"].asInt();
        switch(cmd)
        {
        case REGISTER:
               Register(s, root);
            break;

        case LOGIN:

               Login(s, root);
              break;

        case PIC :
               SetPic(s,root);

            break;



        default:
            break;
        }

    }
}

//客户端可写
void MyServer::writeEvent(TcpSocket *)
{

}

//关闭事件
void MyServer::closeEvent(TcpSocket *s , short )
{
    //用户退出，从用户队列中删除
    //_log->error("客户端退出{}[{}:{}]",userName,s->getIp() , s->getPort());
    std::unique_lock<std::mutex> loc(_userLock);
    std::map<std::string, User*>::iterator it = _users.begin();
    while(it != _users.end())
    {
        if (it->second->getSocket() == s)
        {
            _users.erase(it);
            _log->info("客户端退出{}[{}:{}]",it->second->getName(),s->getIp() , s->getPort());

            //释放user
            delete it->second;
            return;
        }
        it++;
    }
    _log->info("客户端退出[{}:{}]",s->getIp() , s->getPort());

}
//发送数据给客户端

void MyServer::writeData(TcpSocket *s,const Json::Value &inJson)
{
    std::string data = inJson.toStyledString();
    s->writeData(data.c_str(), data.length());
}

//用户注册
void MyServer::Register(TcpSocket *s, const Json::Value &inJson)
{
    int uid_num = 7;

    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();
    std::string sex = inJson["sex"].asString();
    std::string year = inJson["year"].asString();
    std::string month = inJson["month"].asString();
    std::string day = inJson["day"].asString();

    std::string birth = year+'-'+month+'-'+day;



//    _log->info("Register success , user = {}",userName);
//    _log->info("Register success , passwd = {}",passwd);
//    _log->info("Register success , sex = {}",sex);
//    _log->info("Register success , year = {}",year);
//    _log->info("Register success , month = {}",month);
//    _log->info("Register success , day = {}",day);
//    _log->info("Register success , birth = {}",birth);

    std::string number;



    number = Setuid(uid_num);

    while(!Checkuid(number))
    {
        int i_number = atoi(number.c_str());
        i_number++;
        number = to_string(i_number);
        //number = Setuid(uid_num);
    }

    _log->info("Register success , uid = {}",number);

    char sql[100] = {0};
    int result = OK;


        sprintf(sql, "insert into user(uid,passwd,name,sex,birth) values('%s','%s','%s','%s','%s') ",number.c_str() , passwd.c_str() ,userName.c_str(),sex.c_str(),birth.c_str());
        bool ret = _db->db_exec(sql);
        if(!ret)
        {
            result = ERROR;
            _log->error("Register insert select user error");
        }
        else
        {
            _log->info("Register success , user = {}",userName);
            _log->info("Register success , uid = {}",number);
            _log->info("Register success , passwd = {}",passwd);
            _log->info("Register success , sex = {}",sex);
            _log->info("Register success , birth = {}",birth);

        }



    Json::Value json;
    json["cmd"] = REGISTER;
    json["result"] = result;
    json["uid"] = number;
    writeData(s,json);


}

//用户登录
void MyServer::Login(TcpSocket *s, const Json::Value &inJson)
{
    std::string passwd = inJson["passwd"].asString();

    std::string uid = inJson["uid"].asString();

    std::string userName;

    std::string sex;

    std::string birth;

    std::string pic;


    int rank = 0;
    //检测用户是否已经注册过
    char sql[100] = {0};

    sprintf(sql, "select uid,name,sex,birth from user where uid = '%s' and passwd = '%s'; ", uid.c_str() , passwd.c_str());

    printf("exec qian\n");
    int result = OK;
    Json::Value outJson;
    bool ret = _db->db_select(sql,outJson);

    if(!ret)
    {
        result = ERROR;
        _log->error("login select user error");
    }

    //用户已存在，表明注册过了
    if(outJson.isMember("uid"))
    {
       std::unique_lock<std::mutex> lock(_userLock);
       //查看用户是否登录
       if(_users.find(uid) != _users.end()) //说明已经登录
           result = USERLOGIN;

       else
       {
           printf("hello\n");

           int i=0;

           //从outjson获取名字和性别
           userName = outJson["name"][i].asString().c_str();
           sex = outJson["sex"][i].asString().c_str();
           birth = outJson["birth"][i].asString().c_str();

           User* user = new User(uid,userName, passwd , sex,birth, s);


           //将登录后的用户加入map在线用户容器中
           _users.insert(make_pair(uid,user));
           _log->info("用户{}[{}:{}] login", uid , s->getIp(),s->getPort());
           s->setUserName(userName);
           s->setUserId(uid);
           s->setUserSex(sex);
           s->setUserBirth(birth);
       }
    }
    else
      result = NAMEORPASSWD;



    Json::Value json;
    json["cmd"]    = LOGIN;
    json["result"] = result;
    json["uid"] = uid;
    json["name"] = userName ;
    json["sex"] = sex;
    json["birth"] = birth;



    writeData(s,json);

}


void MyServer::SetPic(TcpSocket *s, const Json::Value &inJson)
{


}

//随机获取uid
std::string MyServer::Setuid(int n)
{
    /*初始化*/
        std::string str;                 /*声明用来保存随机字符串的str*/
        char c;                     /*声明字符c，用来保存随机生成的字符*/
        int idx;                    /*用来循环的变量*/
        /*循环向字符串中添加随机生成的字符*/
        for(idx = 0;idx < n;idx ++)
        {
            c = '0' + rand()%10;
            str.push_back(c);       /*push_back()是string类尾插函数。这里插入随机字符c*/
        }


    return str;
}

//int转换为string
std::string MyServer::to_string(int n)
{
    int m = n;
        char s[100];
        char ss[100];
        int i=0,j=0;
        if (n < 0)// 处理负数
        {
            m = 0 - m;
            j = 1;
            ss[0] = '-';
        }
        while (m>0)
        {
            s[i++] = m % 10 + '0';
            m /= 10;
        }
        s[i] = '\0';
        i = i - 1;
        while (i >= 0)
        {
            ss[j++] = s[i--];
        }
        ss[j] = '\0';
        return ss;
}


//检查uid是否重复
int MyServer::Checkuid(std::string uid)
{
    char sql[100] = {0};
    //printf("checking1\n");
    sprintf(sql, "select uid from user where uid = '%s'; ",uid.c_str());
    int result = OK;
    Json::Value outJson;
    printf("checking2\n");

    bool ret = _db->db_select(sql,outJson);
    printf("checking3\n");
    if(!ret)
    {
        result = ERROR;
        _log->error("Register select user error");
    }

    //用户已存在，表明uid注册过了
    if(outJson.isMember("uid"))
            return 0;
    else
    {
        return 1;
    }


}




