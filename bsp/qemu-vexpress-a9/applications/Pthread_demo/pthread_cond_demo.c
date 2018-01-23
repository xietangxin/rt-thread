/*
 * 生产者通过pthread_cond_signal()发信号给消费者
 * 消费者通过pthread_cond_wait()阻塞等待
 */

#include <pthread.h>
#include <finsh.h>
#include <unistd.h>
#include <rtthread.h>

static pthread_mutex_t mutex;
static pthread_cond_t cond;

static pthread_t tid1;
static pthread_t tid2;

static void check_result(char *str, int result)
{
    if (0 == result) {
        rt_kprintf("%s successfully\n", str);
    } else {
        rt_kprintf("%s failed ,errno %d\n", str, result);
    }
}

struct node {
    int n_number;
    struct node *n_next;
};

struct node *head = NULL;

static void *consumer(void *paramter)
{
    struct node *p_node = NULL;

    pthread_mutex_lock(&mutex);

    while (1) {
        while (head == NULL)
            pthread_cond_wait(&cond, &mutex);
        p_node = head;
        head = head->n_next;
        rt_kprintf("consume %d\n", p_node->n_number);
        rt_free(p_node);
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}


static void *producer(void *paramter)
{
    struct node *p_node;
    int count = 0;
    while (1) {
        p_node = (struct node *)rt_malloc(sizeof(struct node));
        if (p_node) {
            p_node->n_number = count++;
            pthread_mutex_lock(&mutex);
            p_node->n_next = head;
            head = p_node;
            pthread_mutex_unlock(&mutex);
            rt_kprintf("produce %d\n", p_node->n_number);
            pthread_cond_signal(&cond);
            sleep(2);
        } else {
            rt_kprintf("malloc struct node failed\n");
        }
    }
    return 0;

}


int pthread_c_demo(void)
{
    int result;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    result = pthread_create(&tid1, NULL, consumer, NULL);
    check_result("thread1", result);
    result = pthread_create(&tid2, NULL, producer, NULL);
    check_result("thread2", result);
    return 0;
}

//FINSH_FUNCTION_EXPORT(pthread_c_demo, pthread condtion demo);
