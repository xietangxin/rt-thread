/*
 * 线程1从消息队列读取消息， 线程2和线程3 向下消息队列发送消息
 */
#include <pthread.h>
#include <finsh.h>
#include <unistd.h>
#include <mqueue.h>

static pthread_t tid1;
static pthread_t tid2;
static pthread_t tid3;

static mqd_t mq;

static void *thread1_entry(void *paramter)
{
    char buf[128];
    int result;
    while (1) {
        result = mq_receive(mq, buf, sizeof(buf), 0);
        if (result != -RT_EFULL) {
            rt_kprintf("thread1 recv %s\n", buf);
        }
    }
    return 0;
}

static void *thread2_entry(void *paramter)
{
    char buf[] = "message2 NO.x";
    int i, result;
    while (1) {
        for (i = 0; i < 10; i++) {
            buf[sizeof(buf) -2] = '0' + i;
           // rt_kprintf("thread2 send %s\n", buf);
            result = mq_send(mq, buf, sizeof(buf), 0);
            if (result == -RT_EFULL) {
                rt_kprintf("thread2 message queue is full\n");
                sleep(20);
            }
        }
        sleep(20);
    }
    return 0;
}

static void *thread3_entry(void *paramter)
{
    char buf[] = "message3 NO.x";
    int i, result;
    while (1) {
        for (i = 0; i < 10; i++) {
            buf[sizeof(buf) -2] = '0' + i;
            //rt_kprintf("thread3 send %s\n", buf);
            result = mq_send(mq, buf, sizeof(buf), 0);
            if (result == -RT_EFULL) {
                rt_kprintf("thread3 message queue is full\n");
                sleep(20);
            }
        }
        sleep(20);
    }
    return 0;
}


int mqueue_demo(void)
{
    struct mq_attr mqstat;
    rt_memset(&mqstat, 0, sizeof(mqstat));
    mqstat.mq_maxmsg = 128;
    mqstat.mq_msgsize = 128;
    mqstat.mq_flags = 0;
    mq = mq_open("mq1", O_CREAT, 0777, &mqstat);
    pthread_create(&tid1, NULL, thread1_entry, NULL);
    pthread_create(&tid2, NULL, thread2_entry, NULL);
    pthread_create(&tid3, NULL, thread3_entry, NULL);
    return 0;
}

//FINSH_FUNCTION_EXPORT(mqueue_demo, message queue demo);

