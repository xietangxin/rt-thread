/*
 *初始化两个静态定时器，一个单次定时，一个周期定时
 *
 */

#include <finsh.h>

static struct rt_timer timer1 ;
static struct rt_timer timer2 ;

static void timeout1(void *paramter)
{
    rt_kprintf("periodic timer is timeout!\n");
}

static void timeout2(void *paramter)
{
    rt_kprintf("one shot timer is timeout!\n");
}

int timer_init(void)
{
    rt_timer_init(&timer1, "timer1", timeout1, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);
    rt_timer_init(&timer2, "timer2", timeout2, RT_NULL, 1000, RT_TIMER_FLAG_ONE_SHOT);
    rt_timer_start(&timer1);
    rt_timer_start(&timer2);
    return 0;
}

//FINSH_FUNCTION_EXPORT(timer_init, init timer demo);
