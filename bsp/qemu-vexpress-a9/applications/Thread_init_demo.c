/*
 *初始化两个静态线程，他们拥有相同的执行函数，但参数不同
 *
 */

#include <rtthread.h>
#include <finsh.h>

#define THREAD_PRIORITY    25
#define THREAD_STACK_SIZE  512
#define THREAD_TIMESLICE   5

static struct rt_thread thread1;
static struct rt_thread thread2;
ALIGN(4);
static rt_uint32_t thread1_stack[THREAD_STACK_SIZE];
static rt_uint32_t thread2_stack[THREAD_STACK_SIZE];

static void thread_entry(void *paramter)
{
    rt_uint32_t count = 0;
    while (1) {
        rt_kprintf("thread%d count:%d\n", (rt_uint32_t)paramter, count++);
        rt_thread_delay(1000);
    }
}

int thread_init()
{
    rt_err_t result;

    result = rt_thread_init(&thread1, "t1", /* 线程名 */ 
        thread_entry, (void *)1,  /* 线程入口函数和函数参数 */
        thread1_stack, sizeof(thread1_stack), /* 线程栈起始地址和大小 */
        THREAD_PRIORITY, THREAD_TIMESLICE);
    if (result == RT_EOK) 
        rt_thread_startup(&thread1);
    else {
        rt_kprintf("init thread1 failed!\n");
        return -1;
    }

    result = rt_thread_init(&thread2, "t2", /* 线程名 */ 
        thread_entry, (void *)2,  /* 线程入口函数和函数参数 */
        thread2_stack, sizeof(thread2_stack), /* 线程栈起始地址和大小 */
        THREAD_PRIORITY, THREAD_TIMESLICE);
    if (result == RT_EOK) 
        rt_thread_startup(&thread2);
    else {
        rt_kprintf("init thread2 failed!\n");
        return -1;
    }
    return 0;
}

FINSH_FUNCTION_EXPORT(thread_init, init 2 thread);
