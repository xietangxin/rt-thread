/*
 * 这个程序会创建2个动态线程：
 * 他们拥有相同的入口函数，相同的优先级
 * 但是他们的入口参数不同
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static void thread_entry(void *paramter)
{
    rt_uint32_t count = 0;

    while(1) {
        rt_kprintf("thread%d count: %d\n", (rt_uint32_t)paramter, count++);
        rt_thread_delay(1000);
    }
}

int rtthread_init()
{
    tid1 = rt_thread_create("t1", thread_entry, (void *)1, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL) {
        rt_thread_startup(tid1);
    } else {
        rt_kprintf("rt_thread_create t1 failed!\n");
        return -1;   
    }


    tid2 = rt_thread_create("t2", thread_entry, (void *)2, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL) {
        rt_thread_startup(tid2);
    } else {
        rt_kprintf("rt_thread_create t2 failed!\n");
        return -1;
    }
    return 0;
}

FINSH_FUNCTION_EXPORT(rtthread_init, create 2 thread test);
