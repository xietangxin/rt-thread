/*
 * 创建一个线程，在线程中申请动态内存，并输出内存地址
 */

#include <rtthread.h>
#include <finsh.h>

static struct rt_thread thread;
static rt_uint32_t thread_stack[512];

static void thread_entry(void *paramter)
{
    rt_uint8_t *ptr[20];
    rt_uint8_t i;
    for (i = 0; i < 20; i++)
        ptr[i] = RT_NULL;
    for (i = 0; i < 20; i++) {
        ptr[i] = rt_malloc(1 << i);
        if (ptr[i] != RT_NULL) {
            rt_kprintf("get memory:0x%x\n", ptr[i]);
        }
    }
    for (i = 0; i < 20; i++) {
        rt_free(ptr[i]);
        ptr[i] = RT_NULL;
    }
}

int memheap(void)
{
    rt_err_t result;
    result = rt_thread_init(&thread, "thread", thread_entry, RT_NULL, thread_stack, sizeof(thread_stack), 20, 10);
    if (result == RT_EOK)
        rt_thread_startup(&thread);
    return 0;
}

//FINSH_FUNCTION_EXPORT(memheap, malloc memory demo);
