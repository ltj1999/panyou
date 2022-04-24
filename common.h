#ifndef COMMON_H
#define COMMON_H

// 5xxx 用户操作指令
#define OK                  5000
#define REGISTER            5001 //注册
#define LOGIN			    5002
#define PIC     			5003




//8xxx 错误

#define ERROR        8001  	//未知错误
#define USEREXIST    8002	//用户已经存在
#define NAMEORPASSWD 8003 	//用户或密码错误
#define USERLOGIN 	 8004	//表示用户已经登录

//其他
#define QUESTION_NUM 5


#endif // REGISTER_H
