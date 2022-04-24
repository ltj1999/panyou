#include <iostream>

#include "thread.h"
#include "tcpserver.h"
#include"myserver.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

//    Thread *pt1=new Thread;
//    Thread *pt2=new Thread;

//    pt1->start();   //测试线程1
//    pt2->start();   //测试线程2


//    while(1)
//    {
//        printf("hi\n");
//        sleep(1);
//    }


    MyServer s;
    s.listen(9999);
    s.start();

     return 0;

}
