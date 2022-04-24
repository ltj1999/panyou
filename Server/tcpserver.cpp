#include "tcpserver.h"


//#include "tcpsocket.h"

TcpServer::TcpServer(int threadNum):m_nextThread(0)
{
    if(threadNum <= 0)
    {
        printf("threadnum <= 0\n");
        exit(-1);
    }
    m_threadNum = threadNum;
    m_threadPool = new Thread[threadNum];
    //创建线程池
    if(m_threadPool == NULL)
    {
        printf("create threadpool error!\n");
        exit(-1);
    }

    m_base = event_base_new();
    if(!m_base)
    {
        printf("could not initialize libevent!\n");
        exit(-1);
    }
}

void TcpServer::listenCb(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *clientAdd,int  , void *data)
{
    printf("有客户端请求链接\n");
    TcpServer *p=(TcpServer*)data;
    p->listenEvent(fd,(struct sockaddr_in*)clientAdd);
}

void TcpServer::listenEvent(evutil_socket_t fd,struct sockaddr_in *clientAddr)
{
    char *ip = inet_ntoa(clientAddr->sin_addr); //获取客户端的IP地址
    uint16_t port = ntohs(clientAddr->sin_port);//获取客户端的端口

    //从线程池中选择一个线程去处理客户端的请求
    //以轮询的方式选择
    struct event_base *base = m_threadPool[m_nextThread].getBase();
    m_nextThread = (m_nextThread + 1)%m_threadNum;
    struct bufferevent *bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        printf("Error constructing bufferevent");
        event_base_loopbreak(base);
        return;
    }


    //创建一个通信对象
    TcpSocket *s = new TcpSocket(this,bev,ip,port);


    //单独封装一个类负责和客户端的通信
    bufferevent_setcb(bev,s->readEventCb,s->writeEventCb,s->closeEventCb, s);
    bufferevent_enable(bev , EV_WRITE); //打开可写
    bufferevent_enable(bev, EV_READ);   //打开可读
    bufferevent_enable(bev, EV_SIGNAL);

    //调用客户端链接函数链接事件

    connectEvent(s);

}


int TcpServer::listen(int port, const char* ip)
{
    struct sockaddr_in sin;

    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    if(ip != NULL)
        inet_aton(ip, &sin.sin_addr);

    m_listener = evconnlistener_new_bind(m_base, listenCb , this,
                             LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                             (struct sockaddr*)&sin,
                             sizeof (sin));
    if(!m_listener)
    {
        printf("could not create a listener!\n");
        return  -1;
    }

    //开启线程池
    for(int i=0; i<m_threadNum ; i++)
    {
        m_threadPool[i].start();
        printf("线程 %d 启动\n",i+1);
    }
    return  0;
}

void TcpServer::start()
{
    event_base_dispatch(m_base);

    evconnlistener_free(m_listener);
    event_base_free(m_base);

    printf("done!\n");
}
