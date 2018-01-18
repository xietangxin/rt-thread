/*
 *  创建两个动态线程，一个静态的邮箱对象
 *  一个线程往邮箱里面发邮件
 *  一个线程从邮箱里面读邮件
 *
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_STACK_SIZE   512
#define THREAD_PRIORITY     25
#define THREAD_TIMESLICE    10

static rt_thread_t t1 = RT_NULL;
static rt_thread_t t2 = RT_NULL;
static struct rt_mailbox mailbox;
static char mb_pool[128];

static rt_uint8_t mb_str1[] = "this is a mail";
static rt_uint8_t mb_str2[] = "this is anther mail";

static void thread1_entry(void *paramter)
{
    rt_thread_delay(100);
    rt_uint32_t *str;
    while(1) {
        rt_kprintf("try to recv a mail\n");
        if (rt_mb_recv(&mailbox, &str, RT_WAITING_FOREVER) == RT_EOK) 
            rt_kprintf("recv mail: %s\n", str);
        rt_thread_delay(100);
    }
}

static void thread2_entry(void *paramter)
{
    rt_uint8_t count = 0;
    while(1) {
        if (count == 0) {
            rt_mb_send(&mailbox, (rt_uint32_t)mb_str1);
        } else if( count == 1 ) {
            rt_mb_send(&mailbox, (rt_uint32_t)mb_str2);
        } else
            return;
        count++;
         rt_thread_delay(100);
    }
}

int mailbox_demo(void)
{
    rt_mb_init(&mailbox, "mb1", mb_pool, sizeof(mb_pool) / 4, RT_IPC_FLAG_FIFO);

    t1 = rt_thread_create("t1", thread1_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    t2 = rt_thread_create("t2", thread2_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(t1);
    rt_thread_startup(t2);
    return 0;
}

//FINSH_FUNCTION_EXPORT(mailbox_demo, mailbox demo);
