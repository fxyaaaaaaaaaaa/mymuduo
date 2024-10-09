
#include "Logger.h"
#include<iostream>
#include"Timestamp.h"
Logger &Logger::instance() {
    static Logger logger;
    return logger;
};
// 设置日志级别
void Logger::setlogLevel(int level)
{
    logLevel_=level;
}
void Logger::log(std::string msg) 
{
    switch (logLevel_)
    {
    case INFO:
        std::cout<<"[INFO]";
        break;
    case FATAL:
        std::cout<<"[FATAL]";
        break;
    case ERROR:
        std::cout<<"[ERROR]";
        break;
    case DEBUG:
        std::cout<<"[DEBUG]";
        break;
    default:
        break;
    }
    //打印时间 和  msg
    std::cout<<Timestamp::now().toString()<<" : "<<msg<<std::endl;
}
