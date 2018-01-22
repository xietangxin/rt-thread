/*
 * 创建两个读线程，一个写线程
 */

#include <pthread.h>
#include <finsh.h>
#include <unistd.h>
#include <rtthread.h>

static pthread_rwlock_t rwlock;

static pthread_t reader1;
static pthread_t reader2;
static pthread_t writer;

static int book = 0;

static void check_result(char *str, int result)
{
    if (0 == result) {
        rt_kprintf("%s successfully\n", str);
    } else {
        rt_kprintf("%s failed ,errno %d\n", str, result);
    }
}


static void *reader1_entry(void *paramter)
{
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        rt_kprintf("reader1 book:%d\n", book);
        sleep(20);
        pthread_rwlock_unlock(&rwlock);
    }
    return 0;
}

static void *reader2_entry(void *paramter)
{
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        rt_kprintf("reader2 book:%d\n", book);
        sleep(20);
        pthread_rwlock_unlock(&rwlock);
    }
    return 0;
}


static void *writer_entry(void *paramter)
{
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        book++;
        pthread_rwlock_unlock(&rwlock);
        rt_kprintf("writer book:%d\n", book);
        sleep(20);
    }
    return 0;
}


int pthread_rw_demo(void)
{
    int result;
    pthread_rwlock_init(&rwlock, NULL);
    result = pthread_create(&reader1, NULL, reader1_entry, NULL);
    check_result("reader1", result);
    result = pthread_create(&reader2, NULL, reader2_entry, NULL);
    check_result("reader2", result);
    result = pthread_create(&writer, NULL, writer_entry, NULL);
    check_result("writer", result);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_rw_demo, pthread condtion demo);
