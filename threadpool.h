#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
typedef struct Task
{
    void (*function)(void * arg); //任务要工作的函数
    void * arg; //任务参数
}Task;

typedef struct ThreadPool
{
    /* data */
    //任务队列
    Task * taskQ;
    int queueCapacity;
    int queueSize;
    int queueFront;
    int queueRear;


    //线程池
    pthread_t managerID; //管理者线程
    pthread_t * threadIDs;  //工作者线程数组

    int minNum; //最少工作线程数
    int maxNum; //最多工作线程数

    int busyNum; //减少锁的粒度，因为忙的线程一直会变，所以可以不频繁锁住整个线程池
    int liveNum; //存活的线程数，就是目前还有多少
    int exitNum; //需要杀死的线程数

    //锁
    pthread_mutex_t mutexPool;  //锁线性池
    pthread_mutex_t mutexBusy; //锁busyNum

    //条件变量，生产者与消费者
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;

    //开关
    int shutdown;
}ThreadPool;
//创建线程池
ThreadPool *threadPoolCreate(int min, int max, int queueSize);
// 销毁线程池
int threadPoolDestroy(ThreadPool* pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);

// 获取线程池中工作的线程的个数
int threadPoolBusyNum(ThreadPool* pool);

// 获取线程池中活着的线程的个数
int threadPoolAliveNum(ThreadPool* pool);

//////////////////////
// 工作的线程(消费者线程)任务函数
void* worker(void* arg);
// 管理者线程任务函数
void* manager(void* arg);
// 单个线程退出
void threadExit(ThreadPool* pool);
#endif

