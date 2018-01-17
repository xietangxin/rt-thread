/*
 *创建两个动态定时器，一个单次定时，一个周期定时
 *
 */

#include <finsh.h>

static rt_timer_t timer1 = RT_NULL;
static rt_timer_t timer2 = RT_NULL;

void timeout1(void *paramter)
{
    rt_kprintf("periodic timer is timeout!\n");
}

void timeout2(void *paramter)
{
    rt_kprintf("one shot timer is timeout!\n");
}

int timer_create(void)
{
    timer1 = rt_timer_create("timer1", timeout1, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
    timer2 = rt_timer_create("timer2", timeout2, RT_NULL, 1000, RT_TIMER_FLAG_ONE_SHOT);
    if (timer2 != RT_NULL)
        rt_timer_start(timer2);
    return 0;
}

//FINSH_FUNCTION_EXPORT(timer_create, create timer demo);
