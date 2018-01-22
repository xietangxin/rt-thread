/*
 * 消费者接受full_sem后，处理共享数据
 * 生产者接受empty_sem后，生产数据
 */

#include <pthread.h>
#include <finsh.h>
#include <unistd.h>
#include <semaphore.h>

static pthread_t tid1;
static pthread_t tid2;

static pthread_mutex_t mutex;
static sem_t full_sem, empty_sem;

struct node {
    int number;
    struct node *next;
};

struct node *head;

static void *producer(void *paramter)
{
    int count = 1;
    struct node *p_node;
    while (1) {
        p_node = (struct node*)rt_malloc(sizeof(struct node));
        if (p_node != NULL) {
            p_node->number = count++;
            pthread_mutex_lock(&mutex);
            p_node->next = head;
            head = p_node;
            pthread_mutex_unlock(&mutex);
            rt_kprintf("produce %d\n", head->number);
            sem_post(&full_sem);
            sleep(20);
            sem_wait(&empty_sem);
        } else {
            rt_kprintf("malloc p_node failed\n");
            return 0;
        }
    }
    return 0;
}

static void *consumer(void *paramter)
{
    struct node *p_node;
    while(1) {
        sem_wait(&full_sem); 
        pthread_mutex_lock(&mutex);
        while (head != NULL) {
            p_node = head;
            rt_kprintf("consume %d\n", p_node->number);
            free(p_node);
            head = head->next;
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
        sleep(20);
    }
    return 0;
}

int sem_demo(void)
{
    sem_init(&empty_sem, 0, 0);
    sem_init(&full_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    return 0;
}

//FINSH_FUNCTION_EXPORT(sem_demo, sem wait and post demo);
