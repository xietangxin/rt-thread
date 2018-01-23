/*
 * 这个程序会创建2个动态线程：
 * 低优先级的t1会先挂起自己
 * 高优先级的t2会在一定时刻唤醒t1
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static void thread1_entry(void *paramter)
{
    rt_kprintf("thread1 startup!\n");
    rt_kprintf("thread1 will suspend itself!\n");
    rt_thread_suspend(tid1);
    //rt_thread_yield(); 使用这个函数让出处理器，但是还是这个线程执行, 可能这时thread2还没有创建
    rt_schedule();
    rt_kprintf("thread1 resume!\n");
}

static void thread2_entry(void *paramter)
{
    rt_thread_delay(1000);
    rt_kprintf("thread2 startup!\n");
    rt_thread_resume(tid1);
    rt_thread_delay(1000);
}

int rtthread_resume()
{
    tid1 = rt_thread_create("t1", thread1_entry, (void *)1, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL) {
        rt_thread_startup(tid1);
    } else {
        rt_kprintf("rt_thread_create t1 failed!\n");
        return -1;   
    }

    tid2 = rt_thread_create("t2", thread2_entry, (void *)2, THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL) {
        rt_thread_startup(tid2);
    } else {
        rt_kprintf("rt_thread_create t2 failed!\n");
        return -1;
    }
    return 0;
}

//FINSH_FUNCTION_EXPORT(rtthread_resume, thread2 resume thread1);
