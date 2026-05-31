#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <string>
#include<arpa/inet.h>
#include<arpa/inet.h>
#include<string.h>
#include<functional>
#include"InetAddr.hpp"
#include "Logger.hpp"

using callback_t = std::function<void(int sockfd,std::string message,InetAddr addr)>;


static const int gdefaultsockfd = -1;
class ChatServer
{
public:
    ChatServer(uint16_t port,callback_t cb)
        : _port(port),
          _sockfd(gdefaultsockfd),
          _isrunning(false),
          _cb(cb)
    {

    }
    void Init()
    {
        // 1.创建socket fd
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 只是把文件在网络中打开
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";
            exit(1);
        }
        LOG(LogLevel::INFO) << "create socket success : " << _sockfd; // 3

        InetAddr local(_port);
        //2.2和套接字进行绑定
        int n = bind(_sockfd,local.Addr(),local.Length());
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind socket success" << _sockfd;
    }
    void Start()
    {
        //所有的服务器都是死循环
        _isrunning = true;
        while(_isrunning)
        {
            //
            char buffer[1024];
            buffer[0] = 0;//清空缓冲区
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            //1.读取数据
            ssize_t n = recvfrom(_sockfd,buffer,sizeof(buffer),0,
            (struct sockaddr*)&peer,&len);
            if(n >0)
            {
                //约定：聊天消息
                buffer[n] = 0;
                //得到对应的client是谁？
                InetAddr clientaddr(peer);
                LOG(LogLevel::DEBUG) << "get a client info: " << 
                clientaddr.Ip() << "-" << clientaddr.Port() << ":" << buffer;
        
                std::string message = buffer;

                //回调！
                _cb(_sockfd,message,clientaddr);

            }

        }
        _isrunning = false;
    }
    void Stop()
    {
        _isrunning = false;
    }
    ~ChatServer() {}

private:
    int _sockfd;
    uint16_t _port;
    // std::string _ip; // 暂时
    callback_t _cb;

    bool _isrunning;
};