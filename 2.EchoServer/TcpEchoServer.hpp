#pragma once

#include<iostream>
#include<string>
#include<cstdlib>
#include<cstring>
//套接字四大金刚
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include"Comm.hpp"
#include"Logger.hpp"

static const int gdefaultfd = -1;
static const int gbacklog = 8;
static const int gport = 8080;

class TcpEchoServer
{
public:
    TcpEchoServer(uint16_t port = gport)
    :_listensockfd(gdefaultfd),_port(port)
    {}
    void Init()
    {
        //1.创建套接字
        _listensockfd = socket(AF_INET,SOCK_STREAM,0);
        if(_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "Create tcp socket error";
            exit(SOCKET_CREATE_ERR);
        }
        LOG(LogLevel::INFO) << "Create tcp socket success" << _listensockfd;

        //2.绑定信息
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        
        if(bind(_listensockfd,(struct sockaddr*)&local,sizeof(local)) != 0)
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(SOCKET_BIND_ERROR);
        }
        LOG(LogLevel::INFO) << "bind socket success" << _listensockfd;

        //3.set socket listen
        //一个TCP服务器一旦设置为listen状态后，启动之后，服务器已经算是运行了
        if(listen(_listensockfd,gbacklog) != 0)
        {
            LOG(LogLevel::FATAL) << "Listen socket error";
            exit(SOCKET_LISTEN_ERR);
        }    
        LOG(LogLevel::INFO) << "Listen socket success" << _listensockfd;
        
    }
    void Start()
    {
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = accept(_listensockfd,(struct sockaddr*)&peer,&len);
            if(sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept client error";
                continue;
            }
            LOG(LogLevel::INFO) << "获取新链接成功" << sockfd;
        }
    }
    ~TcpEchoServer(){}
private:
    int _listensockfd;//暂时
    uint16_t _port;

};