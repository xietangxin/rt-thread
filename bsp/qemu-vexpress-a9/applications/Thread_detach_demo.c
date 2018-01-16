/*
 *线程脱离
 *创建两个线程，在t2中对t1进行脱离操作
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
    rt_thread_detach(&thread1);
    rt_kprintf("detach thread1\n");
    rt_thread_delay(10);
}

int thread_detach(void)
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

//FINSH_FUNCTION_EXPORT(thread_detach, thread 2 detach thread 1);
