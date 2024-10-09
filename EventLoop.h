#pragma once

#include <functional>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include"CurrentThread.h"
#include "noncopyable.h"
#include "Channel.h"
#include"Timestamp.h"
class Poller;
class Channel;
class EventLoop
{
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();
    //开启事件循环
    void loop();
    //退出事件循环
    void quit();

    Timestamp pollReturnTime()const {return pollReturnTime_;}
    //在当前loop中执行cb
    void runInLoop(Functor cb);
    //把cb放入队列中，唤醒loop所在线程，执行cb
    void queueInLoop(Functor cb);
    //用来唤醒loop所在的线程的
    void wakeup();
    //EventLoop的方法---->Poller方法
    void updateChannel(Channel*Channel);
    void removeChannel(Channel*Channel);
    bool hasChannel(Channel*Channel);
    //判断EventLoop对象是否在自己的线程里面
    bool isInLoopThread()const {return threadId_ ==CurrentThread::tid();}
private:
    void handleRead();//wake up
    void doPendingFunctors();//执行回调
    using ChannelList = std::vector<Channel *>;
    std::atomic_bool looping_; // 原子操作,通过CAS实现的
    std::atomic_bool quit_;    // 标识退出loop循环
    std::atomic_bool callingPendingFunctors_;//标识当前loop是否有需要执行的回调操作
    const pid_t threadId_;//记录当前loop所在的线程的id
    Timestamp pollReturnTime_;//poller返回发生事件的channels的时间点
    std::unique_ptr<Poller>poller_;

    int wakeupFd_;//主要作用，当mainLoop获取一个新用户的channel，通过轮询算法选中一个subloop，通过该成员变量配合eventfd来唤醒subloop线程
    std::unique_ptr<Channel> wakeupChannel_;


    ChannelList activeChannels_;
    Channel *currentActiveChannel_;

    std::vector<Functor>pendingFunctors_;//存储loop需要执行的所有的回调操作
    std::mutex mutex_;//互斥锁，用来保护上面vector容器的线程安全操作
};