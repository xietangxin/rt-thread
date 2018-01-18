/*
 * 关闭中断，实现对全局变量的安全访问
 *
 */
#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY     25
#define THREAD_STACK_SIZE   512
#define THREAD_TIMESLICE    5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
static rt_uint32_t count = 0;

static void thread_entry(void *paramter)
{
    rt_uint32_t no = (rt_uint32_t)paramter;
    rt_uint32_t level;

    while(1)
    {
      //  level = rt_hw_interrupt_diable();
        count += no;
       // rt_hw_interrupt_enable(level);
        rt_kprintf("thread[%d] count: %d\n", no, count);
    }
}

int interrupt_lock(void)
{
    tid1 = rt_thread_create("tid", thread_entry, (void *)10, THREAD_STACK_SIZE, THREAD_PRIORITY -5 , THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid2 = rt_thread_create("tid2", thread_entry, (void *)20, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    return 0;
}

FINSH_FUNCTION_EXPORT(interrupt_lock, interrupt lock demo);
