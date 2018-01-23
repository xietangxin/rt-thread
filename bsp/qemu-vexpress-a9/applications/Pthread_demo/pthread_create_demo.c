/*
 * 使用posix接口 创建线程
 *
 */

#include <pthread.h>
#include <finsh.h>

static pthread_t tid1;
static pthread_t tid2;

static void check_result(char *str, int result)
{
    if ( 0 == result ) {
        rt_kprintf("%s successfully\n", str);
    } else {
        rt_kprintf("%s failed, errno %d\n", str, result);
    }
}

static void *thread_entry(void *paramter)
{
    int count = 0;

    while (1) {
        rt_kprintf("thread%d count: %d\n", (int)paramter, count++);
        sleep(2);
    }
    return 0;
}

int pthread_demo(void)
{
    int result;
    result = pthread_create(&tid1, NULL, thread_entry, (void *)1);
    check_result("thread1", result);
    result = pthread_create(&tid2, NULL, thread_entry, (void *)2);
    check_result("thread2", result);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_demo, pthread create demo);

