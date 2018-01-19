/*
 * 创建三个线程
 * 一个线程从消息队列读取消息
 * 一个线程向消息队列发送消息
 * 一个线程向消息队列发送紧急消息
 *
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_STACK_SIZE   512
#define THREAD_PRIORITY     25
#define THREAD_TIMERSLICE   10

static rt_thread_t t1 = RT_NULL;
static rt_thread_t t2 = RT_NULL;
static rt_thread_t t3 = RT_NULL;

static struct rt_messagequeue mq;

static char msg_pool[1024];

static void thread1_entry(void *paramter)
{
    char buf[128];
    rt_err_t result;
    while(1) {
        rt_memset(buf, 0, sizeof(buf));
        rt_kprintf("thread1 recv message:");
        result = rt_mq_recv(&mq, buf, sizeof(buf), RT_WAITING_FOREVER);
        if (result == RT_EOK)
            rt_kprintf(" %s\n", buf);
        rt_thread_delay(10);
    }
}

static void thread2_entry(void *paramter)
{
    char buf[] = "this is a normal message NO.x";
    rt_uint8_t i;
    rt_err_t result;
    while(1) {
        for (i = 0; i < 10; i++) {
            buf[sizeof(buf) -   2] = '0' + i;
            result = rt_mq_send(&mq, buf, sizeof(buf));
            if (result == -RT_EFULL) {
                rt_kprintf("message queue is full ,will delay 1s\n");
                rt_thread_delay(100);
            }
        }
        rt_thread_delay(10);
    }
}

static void thread3_entry(void *paramter)
{
    char buf[] = "this is a urgent message";
    while(1) {
        rt_mq_urgent(&mq, buf, sizeof(buf));
        rt_thread_delay(10);
    }
}

int mq_demo()
{
    rt_err_t result;
    result = rt_mq_init(&mq, "mq1", msg_pool, 128 - sizeof(void *), sizeof(msg_pool), RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
        rt_kprintf("init message queue failed\n");
    t1 = rt_thread_create("t1", thread1_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMERSLICE);
    t2 = rt_thread_create("t2", thread2_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMERSLICE);
    t3 = rt_thread_create("t3", thread3_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMERSLICE);
    rt_thread_startup(t1);
    rt_thread_startup(t2);
    rt_thread_startup(t3);
    return 0;
}

FINSH_FUNCTION_EXPORT(mq_demo, message queue demo);
