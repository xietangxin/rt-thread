/*
 * 创建三个线程 1 2 3
 * 线程1 等待事件3,5(and / or)
 * 线程2 发送事件3
 * 线程3 发送事件5
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_STACK_SIZE   512
#define THREAD_PRIORITY     25
#define THREAD_TIMESLICE    10

static rt_thread_t t1 = RT_NULL;
static rt_thread_t t2 = RT_NULL;
static rt_thread_t t3 = RT_NULL;

static rt_event_t event = RT_NULL;

static void thread1_entry(void *paramter) 
{
    rt_err_t result;
    rt_uint32_t e;
    while(1) {
        result = rt_event_recv(event, (1 << 3) | (1 << 5), RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
        if (result == RT_EOK)
            rt_kprintf("thread1 recv event 3 and 5\n");
        rt_thread_delay(100);
        result = rt_event_recv(event, (1 << 3) | (1 << 5), RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
        if (result == RT_EOK)
            rt_kprintf("thread1 recv event 3 or  5\n");
        rt_thread_delay(100);
    }
}

static void thread2_entry(void *paramter)
{
    rt_thread_delay(50);
    while(1) {
        rt_kprintf("thread2 send event 3!\n");
        rt_event_send(event, 1 << 3); 
        rt_thread_delay(100);
    }
}

static void thread3_entry(void *paramter)
{
    rt_thread_delay(60);
    while(1) {
        rt_kprintf("thread3 send event 5!\n");
        rt_event_send(event, 1 << 5); 
        rt_thread_delay(100);
    }
}

int event_demo(void)
{
    event = rt_event_create("event", RT_IPC_FLAG_FIFO);
    t1 = rt_thread_create("t1", thread1_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (t1 != RT_NULL)
        rt_thread_startup(t1);
    t2 = rt_thread_create("t2", thread2_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (t2 != RT_NULL)
        rt_thread_startup(t2);
    t3 = rt_thread_create("t3", thread3_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (t3 != RT_NULL)
        rt_thread_startup(t3);
    return 0;
}

//FINSH_FUNCTION_EXPORT(event_demo, envent demo);

