#include<mymuduo/TcpServer.h>
#include<string>
#include<functional>
#include<mymuduo/Logger.h>
class  EchoServer
{
public:
    EchoServer(EventLoop*loop,const InetAddress&addr,const std::string name)
              :server_(loop,addr,name)
              ,loop_(loop)
    {
        server_.setConnectionCallback(bind(&EchoServer::onConnection,this,std::placeholders::_1));
        server_.setMessageCallback(bind(&EchoServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        server_.setThreadNum(3);
    }
    void start()
    {
        server_.start();
    }
private:
    TcpServer server_;
    EventLoop* loop_;
    void onConnection(const TcpConnectionPtr&conn)
    {
        if(conn->connected())
        {
            LOG_INFO("Connection UP : %s",conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connection Down : %s",conn->peerAddress().toIpPort().c_str());
        }
    }
    void onMessage(const TcpConnectionPtr&conn,Buffer*buf,Timestamp time)
    {
        std::string msg=buf->retrieveAllAsString();
        conn->send(msg);
        conn->shutdown();
    }
};
int main()
{
    EventLoop loop;
    InetAddress address(9000);
    EchoServer server(&loop,address,"EchoServer001");
    server.start();
    loop.loop();
}