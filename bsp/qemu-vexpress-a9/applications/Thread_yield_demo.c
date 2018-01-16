/*
 *创建两个相同优先级的线程，他们通过rt_thread_yield()接口
 *将处理器让给对方执行
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY     25
#define THREAD_STACK_SIZE   512
#define THREAD_TIMESLICE    5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static void thread1_entry(void *paramter)
{
    rt_uint32_t count = 0;
    while (1) {
        rt_kprintf("thread1 count: %d\n", count++);
        rt_thread_yield();
    }
}

static void thread2_entry(void *paramter)
{
    rt_uint32_t count = 0;
    while (1) {
        rt_kprintf("thread2 count: %d\n", count++);
        rt_thread_yield();
    }
}

int thread_yield(void)
{
    tid1 = rt_thread_create("t1", thread1_entry, NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else {
        rt_kprintf("create thread1 failed\n");
        return -1;
    }
    tid2 = rt_thread_create("t2", thread2_entry, NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else {
        rt_kprintf("create thread2 failed\n");
        return -1;
    }
    return 0;
}


//FINSH_FUNCTION_EXPORT(thread_yield, thread yield test);
