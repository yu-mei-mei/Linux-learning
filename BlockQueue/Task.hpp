#pragma once
#include<iostream>
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
private:
    int a;
    int b;
    int result;
};