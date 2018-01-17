/*
 * 创建一个周期性的动态定时器，然后控制它进行更改定时器的时间长度
 *
 */

#include <finsh.h>

static rt_timer_t timer1;
static rt_uint8_t count;

static void timeout(void *paramter)
{
    rt_kprintf("periodic timer is timeout!\n");
    count++;
    while(count > 8)
    {
        int time_value = 1000;
        rt_timer_control(timer1, RT_TIMER_CTRL_SET_TIME, (void *)&time_value);
    }
}

int timer_control(void)
{
    timer1 = rt_timer_create("timer1", timeout, RT_NULL, 10, RT_TIMER_FLAG_PERIODIC);
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
    return 0;
}

//FINSH_FUNCTION_EXPORT(timer_control, timer control demo);


