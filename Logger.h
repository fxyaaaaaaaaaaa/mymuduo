#pragma once
#include<string>
//定义日志的级别 INFO  ERROR  FATAL DEBUG
#include"noncopyable.h"
//snprintf 是C语言的函数 用于将格式化数据写入字符数组中 原型:snprintf(char*str,size_t size,const char*format,...);
//str是目标字符数组，用于存储格式化后的字符串 size 是字符数组的大小 format是格式化字符串，用于指定输出的格式。
//...是可变参列表 根据格式化字符串中的转换需要的参数一一传递。
// LOG_INFO("%s %d",arg1,arg2) 为了安全要使用do while 结构；##_VA_ARGS_获取可变参列表的宏
#define LOG_INFO(logmsgFormat,...)\
    do\
    {\
        Logger&logger=Logger::instance();\
        logger.setlogLevel(INFO);\
        char buf[1024]={0};\
        snprintf(buf,1024,logmsgFormat,##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)
    #define LOG_ERROR(logmsgFormat,...)\
    do\
    {\
        Logger&logger=Logger::instance();\
        logger.setlogLevel(ERROR);\
        char buf[1024]={0};\
        snprintf(buf,1024,logmsgFormat,##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)
    #define LOG_FATAL(logmsgFormat,...)\
    do\
    {\
        Logger&logger=Logger::instance();\
        logger.setlogLevel(FATAL);\
        char buf[1024]={0};\
        snprintf(buf,1024,logmsgFormat,##__VA_ARGS__);\
        logger.log(buf);\
        exit(-1);\
    }while(0)
    #ifdef MUDEBUG
    #define LOG_DEBUG(logmsgFormat,...)\
    do\
    {\
        Logger&logger=Logger::instance();\
        logger.setlogLevel(DEBUG);\
        char buf[1024]={0};\
        snprintf(buf,1024,logmsgFormat,##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)
    #else 
        #define LOG_DEBUG(logmsgFormat,...)
    #endif
enum LogLevel
{
    INFO,//普通信息
    ERROR,//错误信息
    FATAL,//core信息
    DEBUG,//调试信息
};
class Logger:  noncopyable
{
public:
    //获取日志的唯一实例对象
    static Logger &instance();
    //设置日志级别
    void setlogLevel(int level);
    //写日志
    void log(std::string msg);
private:
    int logLevel_;//定义变量_放在变量名后面，区分系统定义的变量,系统定义的变量_在变量名之前
    Logger(){};
    ~Logger(){};
};

