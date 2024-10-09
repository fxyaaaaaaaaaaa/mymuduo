#pragma once
#include"noncopyable.h"
#include<vector>
#include<unordered_map>
#include"Timestamp.h"
//muduo库中多路复用分发器的核心IO模块
class Channel;
class EventLoop;
class Poller:noncopyable
{
public:
    using ChannelList=std::vector<Channel*>;

    Poller(EventLoop*loop);
    virtual ~Poller()=default;
    virtual Timestamp poll(int timeoutMs,ChannelList * activeChannels)=0;
    virtual void updateChannel(Channel*channel)=0;
    virtual void removeChannel(Channel*channel)=0;
    //判断参数channel是否在当前Poller当中
    bool hasChannel(Channel*channel) const ;
    //EventLoop可以通过该接口获取默认的IO复用的具体实现
    static Poller* newDefaultPoller(EventLoop*loop);
    protected:
    //map的key代表sockfd value代表所属的channel的类型
    using ChannelMap=std::unordered_map<int,Channel*>;
    ChannelMap channels_;
private:
    EventLoop*ownerLoop_;
};