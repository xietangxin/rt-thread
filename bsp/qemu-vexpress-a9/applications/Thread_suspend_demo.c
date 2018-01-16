/*
 * 创建两个动态线程
 * 低优先级的t1一直运行
 * 高优先级的t2在一定时间后唤醒并挂起线程t1
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY    25
#define THREAD_STACK_SIZE  512
#define THREAD_TIMESLICE   5

static struct rt_thread thread1;
static struct rt_thread thread2;
static rt_uint8_t thread1_stack[THREAD_STACK_SIZE];
static rt_uint8_t thread2_stack[THREAD_STACK_SIZE];


static void thread1_entry(void *paramter)
{
    rt_uint32_t count = 0;
    while (1) {
        rt_kprintf("thread1 count: %d\n", count++);
    }
}

static void thread2_entry(void *paramter)
{
    rt_thread_delay(10);
    rt_thread_suspend(&thread1);
    rt_kprintf("\nin thread2 suspend thread1\n");
    rt_thread_delay(10);
}

int thread_suspend(void)
{
    rt_err_t result;
    result = rt_thread_init(&thread1, "thread1",
        thread1_entry, NULL,
        thread1_stack, sizeof(thread1_stack),
        THREAD_PRIORITY, THREAD_TIMESLICE);
    if (result == RT_EOK)
        rt_thread_startup(&thread1);
    else {
        rt_kprintf("init thread1 failed\n");
        return -1;
    }

    result = rt_thread_init(&thread2, "thread1",
        thread2_entry, NULL,
        thread2_stack, sizeof(thread2_stack),
        THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (result == RT_EOK)
        rt_thread_startup(&thread2);
    else {
        rt_kprintf("init thread2 failed\n");
        return -1;
    }
    return 0;
}

//FINSH_FUNCTION_EXPORT(thread_suspend, thread 2 suspend thread 1);
