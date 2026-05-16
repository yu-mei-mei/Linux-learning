#include <pthread.h>
#include <queue>
#include <functional>
#include <stdio.h>
#include <unistd.h>

// ─── 全局数据 ───
std::queue<std::function<void()>> tasks;          // 任务队列
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 锁 → 保护队列
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // 条件变量 → 队列空时等待

// ─── 消费者：工作线程 ───
void *worker(void *arg)
{
    while (true)
    {
        pthread_mutex_lock(&lock); // ① 加锁

        while (tasks.empty())                // ② 队列空 → 等待
            pthread_cond_wait(&cond, &lock); //   (自动解锁 + 挂起，唤醒后重新加锁)

        auto task = tasks.front(); // ③ 取任务
        tasks.pop();

        pthread_mutex_unlock(&lock); // ④ 解锁（尽快释放，不要在持有锁时执行任务）

        task(); // ⑤ 执行任务（已解锁，不阻塞其他线程取任务）
    }
    return nullptr;
}

// ─── 生产者：放任务 ───
void Equeue(std::function<void()> task)
{
    pthread_mutex_lock(&lock);   // ① 加锁
    tasks.push(task);            // ② 入队
    pthread_cond_signal(&cond);  // ③ 通知一个等待的线程
    pthread_mutex_unlock(&lock); // ④ 解锁
}

// ─── 主函数 ───
int main()
{
    pthread_t threads[3];
    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], nullptr, worker, nullptr); // 创建 3 个工作线程

    Equeue([]()
           { printf("task 1\n"); });
    Equeue([]()
           { printf("task 2\n"); });
    Equeue([]()
           { printf("task 3\n"); });

    sleep(1);
    // 实际需要更完善的退出机制，此处仅演示核心流程
}
