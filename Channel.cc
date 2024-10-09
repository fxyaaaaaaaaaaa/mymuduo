#include "Channel.h"
#include <sys/epoll.h>
#include"EventLoop.h"
#include"Logger.h"
const int Channel::KNoneEvent = 0;
const int Channel::KReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::Channel::KWriteEvent = EPOLLOUT;
Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false)
{
}
Channel::~Channel(){}
//channel 的 tie 方法调用时机
void Channel::tie(const std:: shared_ptr<void>&obj)
{
    tie_ = obj;
    tied_ = true;
}
// 当改变channel所表示的fd的events事件后，update负责在poller里面更改fd相应的事件epoll_ctrl
// EventLoop--->ChannelList Poller
void Channel::update()
{
      // 通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    loop_->updateChannel(this);
}
//在channel所属的EventLoop中 把当前channel删除掉
void Channel::remove()
{
    
     loop_->removeChannel(this);
}
//根据poller通知的channel的具体事件 由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n",revents_);
    //EPOLLHUP对端关闭连接
    if((revents_& EPOLLHUP)&&(revents_ & EPOLLIN))
    {
        if(closeCallback_)
        {
            closeCallback_();
        }
    }
    if(revents_ & EPOLLERR)
    {
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    //EPOLLIN 表示有数据可读的事件 EPOLLPRI表示紧急事件要读
    if(revents_ &(EPOLLIN | EPOLLPRI))
    {
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    //EPOLLOUT 表示有数据可写的事件
    if(revents_ & EPOLLOUT)
    {
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
};
void Channel::handleEvent(Timestamp receiveTime)
{
    if(tied_)
    {
        std::shared_ptr<void>guard=tie_.lock();
        if(guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
};
