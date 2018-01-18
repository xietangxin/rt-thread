/*
 * 创建一个静态内存池，在创建两个线程
 * 一个申请内存， 一个释放内存
 */


#include <rtthread.h>
#include <finsh.h>

#define THREAD_STACK_SIZE   512
#define THREAD_PRIORITY     25
#define THREAD_TIMESLICE    5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static rt_uint8_t *ptr[48];
static rt_uint8_t mempool[4096];
static struct rt_mempool mp;
    
static void thread1_entry(void *paramter)
{
    rt_uint8_t *block;
    rt_uint8_t i;

    while(1) {
        for(i = 0; i < 48; i++) {
            rt_kprintf("allocate NO.%d\n", i);
            if (ptr[i] == RT_NULL)
                ptr[i] = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
        }

        //没有出现内存块不够，线程挂起现象
        block = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
        rt_kprintf("allocate the block mem\n");
        rt_mp_free(block);
        block = RT_NULL;
    }
}

static void thread2_entry(void *paramter)
{
    rt_uint8_t i = 0;
    while(1) {
        for (i = 0; i < 48; i++) {
            rt_kprintf("free mem NO.%d\n", i);
            if (ptr[i] != RT_NULL)
                rt_mp_free(ptr[i]);
            ptr[i] = RT_NULL;
        }
        rt_thread_delay(1000);
    }
}

int mp_init(void)
{
    rt_uint8_t i;
    rt_err_t result;
    for (i = 0; i < 48; i++)
        ptr[i] = RT_NULL;
    result = rt_mp_init(&mp, "mempool1", mempool, sizeof(mempool), 48);
    if (result != RT_EOK)
        rt_kprintf("init mempool failed\n");
    tid1 = rt_thread_create("t1", thread1_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    tid2 = rt_thread_create("t2", thread2_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    return 0;
}

FINSH_FUNCTION_EXPORT(mp_init, mempool init demo);

