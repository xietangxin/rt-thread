#include <pthread.h>
#include <unistd.h>
#include <finsh.h>


static pthread_t tid1;
static pthread_t tid2;

static void check_result(char* str,int result)
{
    if (0 == result) {
        rt_kprintf("%s successfully!\n",str);
    } else {
        rt_kprintf("%s failed! error code is %d\n",str,result);
    }

}
static void* thread1_entry(void* parameter)
{
    int count = 0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while(1) {
        rt_kprintf("thread1 run count: %d\n",count ++);
        sleep(2);    
    }
    return 0;
}

static void* thread2_entry(void* parameter)
{
    int count = 0;
    sleep(8);
    pthread_cancel(tid1);
    pthread_join(tid1,NULL);
    printf("thread1 exited!\n");
    while(1) {
        rt_kprintf("thread2 run count: %d\n",count ++);
        sleep(2);    
    }
    return 0;
}

int cancel_demo()
{
    int result;
    result = pthread_create(&tid1,NULL,thread1_entry,NULL);
    check_result("thread1 created",result);

    result = pthread_create(&tid2,NULL,thread2_entry,NULL);
    check_result("thread2 created",result);

    return 0;
}

FINSH_FUNCTION_EXPORT(cancel_demo, cancel pthread demo);

