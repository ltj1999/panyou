#include "db.h"

DB::DB(const char *host , const char *userName , const char *passwd, const char *dbName)
{
    //初始化数据库句柄
    _mysql = mysql_init(NULL);
    if(_mysql == NULL)
    {
        spdlog::get("TestQt")->error("初始化数据库失败\n");
        exit(-1);
    }

    //链接mysql 服务器
    MYSQL *con =mysql_real_connect(_mysql,host,userName,passwd,dbName,0,NULL,0);
    if(con == NULL)
    {
        spdlog::get("TestQt")->error("连接数据库失败: {} ", mysql_error(_mysql));
        exit(-1);
    }
    _mysql = con;

    //设置字符集
    int ret = mysql_query(_mysql,"set names utf8");
    if(ret != 0)
    {
        spdlog::get("TestQt")->error("设置字符集失败: {} ", mysql_error(_mysql));
        exit(-1);
    }

}


bool DB::db_exec(const char *sql)
{
    std::unique_lock<std::mutex> loc{_mutex};  //数据库句柄上锁
    printf(sql);
    int ret = mysql_query(_mysql , sql);
    if(ret != 0)
    {
        spdlog::get("TestQt")->error("mysql 数据库执行语句失败 : {} ", mysql_error(_mysql));
        return false;
    }
    return  true;
}


bool DB::db_select(const char *sql, Json::Value &outJson)
{

    printf("sql=%s\n",sql);
    std::unique_lock<std::mutex> loc{_mutex};  //数据库句柄上锁
    int ret = mysql_query(_mysql , sql);

    if(ret != 0)
    {
        spdlog::get("TestQt")->error("mysql 数据库执行语句失败 : {} ", mysql_error(_mysql));
        return false;
    }


    //从mysql服务器下载查询结果
    MYSQL_RES *sql_res = mysql_store_result(_mysql);



    if(sql_res == NULL)
    {
        if(mysql_errno(_mysql)== 0)
        {
            return true;
        }
        else
        {
            spdlog::get("TestQt")->error("mysql 数据库执行语句失败 : {} ", mysql_error(_mysql));
        }
    }



    MYSQL_ROW row;              //从结果集中一行一行取出数据
    unsigned int num_fileds = mysql_num_fields(sql_res);  //获取列数
    MYSQL_FIELD *fetch_field = mysql_fetch_field(sql_res); //获取表头



    //一行一行获取数据
    while(row = mysql_fetch_row(sql_res))
    {
        for(unsigned int i=0; i<num_fileds;i++)
        {

            outJson[fetch_field[i].name].append(row[i]);
        }
    }

    mysql_free_result(sql_res);



    return true;
}
