/*
 * 使用posix接口 创建两个线程
 * 线程1运行一段时间，自己退出
 * 线程2等待线程1结束
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
    for (; count < 10; count++) {
        rt_kprintf("thread%d count: %d\n", (int)paramter, count);
        sleep(2);
    }
    pthread_exit(NULL);
    return 0;
}

static void *thread2_entry(void *paramter)
{
    int count = 0;
    /* 阻塞等待线程1结束,回收线程资源 */
    pthread_join(tid1, NULL);
    rt_kprintf("thread%d count: %d\n", (int)paramter, count);
    return 0;
}    

int pthread_e_demo(void)
{
    int result;
    result = pthread_create(&tid1, NULL, thread1_entry, (void *)1);
    check_result("thread1", result);
    result = pthread_create(&tid2, NULL, thread2_entry, (void *)2);
    check_result("thread2", result);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_e_demo, pthread exit  demo);

