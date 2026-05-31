#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"

class Route
{
private:
    bool IsExists(const InetAddr &addr)
    {
        for (auto &user : _online_user)
        {
            if (user == addr)
                return true;
        }
        return false;
    }

    void AddUser(const InetAddr &addr)
    {
        if (!IsExists(addr))
            _online_user.push_back(addr);
    }

    void DeleteUser(const InetAddr &addr)
    {
        auto iter = _online_user.begin();
        for (; iter != _online_user.end(); iter++)
        {
            if (*iter == addr)
            {
                _online_user.erase(iter);
                break;
            }
        }
    }

    // 广播给所有人（包括发送者自己）
    void SendToAll(int sockfd, const std::string &msg)
    {
        for (auto &user : _online_user)
        {
            sendto(sockfd, msg.c_str(), msg.size(), 0, user.Addr(), user.Length());
        }
    }

    // 广播给除 exclude 外的所有人
    void SendToOthers(int sockfd, const std::string &msg, const InetAddr &exclude)
    {
        for (auto &user : _online_user)
        {
            if (user == exclude)
                continue;
            sendto(sockfd, msg.c_str(), msg.size(), 0, user.Addr(), user.Length());
        }
    }

public:
    Route() {}

    // 核心路由函数：接收消息并分发给所有在线用户
    void RouteMessageToAll(int sockfd, std::string &message, const InetAddr &addr)
    {
        LockGuard lockguard(&_lock);

        // 1. 检查是否为新用户
        bool isNewUser = !IsExists(addr);
        AddUser(addr);

        // 2. 新用户加入 → 通知其他人
        if (isNewUser)
        {
            std::string notice = "[系统] " + addr.ToString() + " 加入了聊天室" +
                                 " [当前在线: " + std::to_string(_online_user.size()) + " 人]";
            SendToOthers(sockfd, notice, addr);
            LOG(LogLevel::INFO) << notice;
        }

        // 3. 处理 QUIT 退出
        if (message == "QUIT")
        {
            std::string leaveMsg = "[系统] " + addr.ToString() + " 离开了聊天室" +
                                   " [当前在线: " + std::to_string(_online_user.size() - 1) + " 人]";
            DeleteUser(addr);
            SendToAll(sockfd, leaveMsg);
            LOG(LogLevel::INFO) << leaveMsg;
            return;
        }

        // 4. 正常消息 → 格式化后广播给所有人（含发送者，作为回显确认）
        std::string chatMsg = "[" + addr.ToString() + "] " + message;
        SendToAll(sockfd, chatMsg);
        LOG(LogLevel::DEBUG) << chatMsg;
    }

    void RouteMessageToOne()
    {
        // TODO: 私聊功能（可以后续扩展）
    }

    ~Route() {}

private:
    std::vector<InetAddr> _online_user; // 在线用户列表
    Mutex _lock;                         // 保护用户列表的线程安全
};
