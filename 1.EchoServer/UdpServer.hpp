#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <string>
#include<arpa/inet.h>
#include<arpa/inet.h>
#include<string.h>
#include "Logger.hpp"

static const int gdefaultsockfd = -1;
class UdpServer
{
public:
    UdpServer(uint16_t port)
        : _port(port),
          _sockfd(gdefaultsockfd),
          _isrunning(false)
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

        // 2.绑定信息bind
        // 2.1填充IP和port
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // local.sin_addr.s_addr =inet_addr(_ip.c_str());//1.字符串转整数IP 顺带转化为网络序
        local.sin_addr.s_addr = htonl(INADDR_ANY);//最佳实践任意IP绑定而不是绑定固定IP
        //2.2和套接字进行绑定
        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
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
                //client是谁，IP和端口号给我
                uint16_t clientport =ntohs(peer.sin_port);
                std::string clientip = inet_ntoa(peer.sin_addr);
                
                buffer[n] = 0;
                LOG(LogLevel::DEBUG) << "[" << "clientip:" << ":" << clientport << "]#" << buffer;

                std::string echo_string = "server echo#";
                echo_string += buffer;

                sendto(_sockfd,echo_string.c_str(),echo_string.size(),0,
                (struct sockaddr*)&peer,len);

            }

        }
        _isrunning = false;
    }
    void Stop()
    {
        _isrunning = false;
    }
    ~UdpServer() {}

private:
    int _sockfd;
    uint16_t _port;
    // std::string _ip; // 暂时


    bool _isrunning;
};