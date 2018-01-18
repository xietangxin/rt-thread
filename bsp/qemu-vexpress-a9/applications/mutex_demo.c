/*
 * 测试RT_Thread 的优先级继承策略
 * 创建三个动态线程 1 2 3 ，优先级从高到低
 * 线程3 持有互斥量， 线程2 尝试获取互斥量， 线程1检测线程2,3的优先级是否相同
 *
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_STACK_SIZE   512
#define THREAD_PRIORITY      25
#define THREAD_TIMESLICE    10

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
static rt_thread_t tid3 = RT_NULL;
static rt_mutex_t mutex;

static void thread1_entry(void *paramter)
{
    rt_thread_delay(100);
    if (tid2->current_priority == tid3->current_priority) 
        rt_kprintf("tid2 priority = tid3 priority\n");
}

static void thread2_entry(void *paramter)
{
    rt_thread_delay(10);
    rt_err_t result;

        result = rt_mutex_take(mutex, RT_WAITING_FOREVER);
        if (result == RT_EOK)
            rt_mutex_release(mutex);
}

static void thread3_entry(void *paramter)
{
    rt_err_t result;
        result = rt_mutex_take(mutex, RT_WAITING_FOREVER);
        if (result != RT_EOK)
            rt_kprintf("thread3 take mutex failed!\n");
        rt_thread_delay(1000);
}

int mutex_demo(void)
{
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    if (mutex == RT_NULL)
        rt_kprintf("create mutex failed!\n");
    tid1 = rt_thread_create("tid1", thread1_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    tid2 = rt_thread_create("tid2", thread2_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    tid3 = rt_thread_create("tid3", thread3_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (tid3 != RT_NULL)
        rt_thread_startup(tid3);
    return 0;
}

//FINSH_FUNCTION_EXPORT(mutex_demo, mutex demo);

