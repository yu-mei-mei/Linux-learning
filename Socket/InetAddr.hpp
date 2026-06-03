#pragma once

// 这个类是用来描述client Socket信息的类
// 方便我们后续用它来管理客户端

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define Conv(addr) ((struct sockaddr*)&addr)

class InetAddr
{
private:
    void Net2Host()
    {
        _port = ntohs(_addr.sin_port);
        _ip = inet_ntoa(_addr.sin_addr);
    }
    void Host2Net()
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(_port);
        _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    }

public:
    InetAddr()
    {}
    InetAddr(const struct sockaddr_in &addr) : _addr(addr)
    {
        Net2Host();
    }
    InetAddr(uint16_t port, const std::string &ip = "0.0.0.0") : _ip(ip), _port(port)
    {
        Host2Net();
    }
    void Init(const struct sockaddr_in &addr){
        _addr = addr;
        Net2Host();
    }
    std::string Ip() const
    {
        return _ip;
    }
    uint16_t Port() const
    {
        return _port;
    }
    struct sockaddr* Addr() const
    {
        return Conv(_addr);
    }
    socklen_t Length() const
    {
        return sizeof(_addr);
    }
    std::string ToString() const
    {
        return _ip + "-" + std::to_string(_port);
    }
    bool operator==(const InetAddr &addr) const
    {
        return (_ip == addr._ip && _port == addr._port);
        // return (_ip == addr._ip);
    }


    ~InetAddr() {}

private:
    struct sockaddr_in _addr; // 网络风格地址
    // 主机发风格地址
    std::string _ip;
    uint16_t _port;
};