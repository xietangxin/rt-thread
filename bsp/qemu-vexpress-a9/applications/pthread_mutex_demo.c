/*
 * 创建两个线程
 * 使用互斥锁
 */

#include <pthread.h>
#include <finsh.h>
static pthread_t tid1;
static pthread_t tid2;
static pthread_mutex_t mutex;

static void check_result(char *str, int result)
{
    if ( 0 == result ) {
        rt_kprintf("%s successfully\n", str);
    } else {
        rt_kprintf("%s failed, errno %d\n", str, result);
    }
}

static void printer(char *str)
{
    while (*str != '\0') {
        rt_kprintf("%c ", *str);
        str++;
        sleep(1);
    }
    rt_kprintf("\n");
}

static void *thread1_entry(void *paramter)
{
    char *str = "hello rt_thread";
    pthread_mutex_lock(&mutex);
    printer(str);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    return 0;
}

static void *thread2_entry(void *paramter)
{
    char *str = "HI WORLD";
    pthread_mutex_lock(&mutex);
    printer(str);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    return 0;
}    

int pthread_m_demo(void)
{
    int result;
    pthread_mutex_init(&mutex, NULL);
    result = pthread_create(&tid1, NULL, thread1_entry, NULL);
    check_result("thread1", result);
    result = pthread_create(&tid2, NULL, thread2_entry, NULL);
    check_result("thread2", result);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_m_demo, pthread mutex demo);

