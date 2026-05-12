#include"Logger.hpp"
#include<unistd.h>

int main()
{
    //EnableConsoleLogStrategy();
    EnableFileLogStrategy();

    LOG(LogLevel::ERROR) << "hello world" << ", 3.14" << 123;
    LOG(LogLevel::WARNING)<<"hello world" << ", 3.14" << 123;
    LOG(LogLevel::ERROR) << "hello world" << ", 3.14" << 123;
    LOG(LogLevel::ERROR) << "hello world" << ", 3.14" << 123;
    // std::string test = "hello world,hello log";
    // // //策略1 向显示器打印
    // std::unique_ptr<LogStrategy> logger_ptr = std::make_unique<ConsoleLogStrategy>();
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);
    // logger_ptr->SyncLog(test);

    //2.策略2 向文件打印
    //std::unique_ptr<LogStrategy> logger_ptr = std::make_unique<FileLogStrategy>();
    // logger_ptr->SyncLog(GetCurrentTime());
    // sleep(1);
    // logger_ptr->SyncLog(GetCurrentTime());
    // sleep(1);
    
    // logger_ptr->SyncLog(GetCurrentTime());
    // sleep(1);

    // logger_ptr->SyncLog(GetCurrentTime());
    // sleep(1);

    // logger_ptr->SyncLog(GetCurrentTime());
    // sleep(1);



    return 0;
}