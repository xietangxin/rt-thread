/*
 * 创建两个线程
 * 线程的优先级为8
 */

#include <pthread.h>
#include <unistd.h>
#include <sched.h>
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

static void* thread_entry(void* parameter)
{
    int count = 0;
    int no = (int) parameter; 

    while (1) {
        rt_kprintf("thread%d count: %d\n", no, count ++);
        sleep(20);    
    }
    return 0;
}

int pthread_attr()
{
    int result;
    pthread_attr_t attr;      
    struct sched_param prio;  

    prio.sched_priority = 8;  
    pthread_attr_init(&attr);  
    pthread_attr_setschedparam(&attr,&prio);  

    result = pthread_create(&tid1,&attr,thread_entry,(void*)1);
    check_result("thread1 created",result);

    result = pthread_create(&tid2,NULL,thread_entry,(void*)2);
    check_result("thread2 created",result);

    return 0;
}


FINSH_FUNCTION_EXPORT(pthread_attr, set pthread attr demo);

