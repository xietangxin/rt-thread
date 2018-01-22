/*
 * 使用posix接口 创建两个线程
 * 一个线程自己设置脱离线程
 * 在主线程中设置脱离另一个线程
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

static void *thread1_entry(void *paramter)
{
    int count = 0;
    pthread_detach(pthread_self());
    for (; count < 3; count++) {
        rt_kprintf("thread%d count: %d\n", (int)paramter, count);
        sleep(2);
    }
    return 0;
}

static void *thread2_entry(void *paramter)
{
    int count = 0;
    for (; count < 3; count++) {
        rt_kprintf("thread%d count: %d\n", (int)paramter, count);
        sleep(2);
    }
    return 0;
}    

int pthread_d_demo(void)
{
    int result;
    result = pthread_create(&tid1, NULL, thread1_entry, (void *)1);
    check_result("thread1", result);
    result = pthread_create(&tid2, NULL, thread2_entry, (void *)2);
    check_result("thread2", result);
    pthread_detach(tid2);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_d_demo, pthread detach demo);

