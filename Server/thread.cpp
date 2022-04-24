#include "thread.h"


Thread::Thread()
{
    m_base = event_base_new();
    if(!m_base)
    {
        printf("Couldn't create an event_base:exiting\n");
        exit(-1);
    }

    //创建管道
    int fd[2];
    if(pipe(fd) == -1)
    {
        perror("pipe");
        exit(-1);
    }

    m_pipeReadFd = fd[0];
    m_pipeWriteFd = fd[1];

    //让管道事件监听管道的读端
    //如果监听到 管道的读端有数据可读
    event_set(&m_pipeEvent,m_pipeReadFd,EV_READ | EV_PERSIST, pipeRead,this );

    //将事件添加到_base 集合中
    event_base_set(m_base,&m_pipeEvent);

    //开启事件的监听
    event_add(&m_pipeEvent,0);
}

void Thread::pipeRead(evutil_socket_t,short,void *)
{
    printf("piperead\n");
}

void Thread::start() //线程运行
{
    //创建一个线程
    pthread_create(&m_threadId,NULL,worker,this);
    //线程分离
    pthread_detach(m_threadId);
}

void* Thread::worker(void *arg) //线程的工作函数
{
    Thread* p = (Thread*)arg;
    p->run();

    return NULL;
}

void Thread::run()
{


    printf("%d: start \n",m_threadId); //开启线程
    //监听base事件集合
    //event_base_dispatch 死循环，用于处理事件， 类似与 Qt 的exec()
    //如果m_base 事件集合是空的话，则event_base_dispatch会立马返回
    //初始化的时候 需要给 base 添加事件，让它不空
    event_base_dispatch(m_base);
    event_base_free(m_base);

    printf("%d: done \n",m_threadId);
}


struct event_base *Thread::getBase()
{
    return m_base;
}
