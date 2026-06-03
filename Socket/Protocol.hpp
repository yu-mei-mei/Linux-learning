#pragma once

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

//有效载荷长度    （int）\r\n
//jsonstr\r\n

class Request
{
public:
    Request() {}
    // 序列化对象
    bool Serialize(std::string *out)
    {
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["oper"] = _oper;

        Json::StyledWriter writer;
        *out = writer.write(root);
        if (out->empty())
            return false;
        return true;
    }
    // 反序列话对象
    bool Deserialize(std::string &in)
    {
        Json::Reader reader;
        Json::Value root;
        bool ret = reader.parse(in, root);
        if (!ret)
        {
            return false;
        }

        _x = root["x"].asInt();
        _y = root["y"].asInt();
        _oper = root["oper"].asInt();
        return true;
    }
    ~Request() {}

private:
    // x oper y   约定1
    int _x;
    int _y;
    char _oper;
};

class Response
{
public:
    Response() {}

    bool Serialize(std::string *out)
    {
        Json::Value root;
        root["x"] = _result;
        root["y"] = _code;
        Json::StyledWriter writer;
        *out = writer.write(root);
        if (out->empty())
            return false;
        return true;
    }
    bool Deserialize(std::string &in)
    {
        Json::Reader reader;
        Json::Value root;
        bool ret = reader.parse(in, root);
        if (!ret)
        {
            return false;
        }

        _result = root["result"].asInt();
        _code = root["code"].asInt();

        return true;
    }
    ~Response() {}

private:
    int _result;
    int _code; // 可信度
};