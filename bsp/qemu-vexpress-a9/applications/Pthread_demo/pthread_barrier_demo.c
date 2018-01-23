/*
 * 创建一个屏障，等待三个线程
 */

#include <pthread.h>
#include <unistd.h>
#include <finsh.h>

static pthread_t tid1;
static pthread_t tid2;
static pthread_t tid3;

static pthread_barrier_t barrier;

static void *thread_entry(void *paramter)
{
    rt_kprintf("thread%d arrive barrier\n", (int)paramter);
    pthread_barrier_wait(&barrier);
    rt_kprintf("thread%d leave barrier\n", (int)paramter);
    return 0;
}

int barrier_demo()
{
    pthread_barrier_init(&barrier, NULL, 3);
    pthread_create(&tid1, NULL, thread_entry, (void *)1);
    pthread_create(&tid2, NULL, thread_entry, (void *)2);
    pthread_create(&tid3, NULL, thread_entry, (void *)3);
    return 0;
}

//FINSH_FUNCTION_EXPORT(barrier_demo, pthread barrier demo);
