#pragma once
#include<iostream>
#include<functional>
#include<sstream>

class Task
{
public:
    Task()
    {}
    Task(int x,int y):a(x),b(y)
    {}
    void Execute()
    {
        result = a + b;
    }
    void operator()()
    {
        Execute();
    }
    void Print()
    {
        std::cout << a << "+" << b << " = " << result << std::endl;
    }
    std::string Result2String()
    {
        std::stringstream ss;
        ss << a << "+" << b << " = " << result;
        return ss.str();
    }
private:
    int a;
    int b;
    int result;
};

void PrintLog()
{
    std::cout << "我是一个日志任务" << std::endl;
}