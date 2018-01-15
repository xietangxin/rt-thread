#include <finsh.h>

/*
 * 使用宏的方式向finsh中添加函数和变量，设置函数别名
 */
int var;
int macro_add_func(int a)
{
    rt_kprintf("macro_add_func succeed, %d\n", a);
    return 0;
}

FINSH_FUNCTION_EXPORT(macro_add_func, test add fuc to finsh);
FINSH_VAR_EXPORT(var, finsh_type_int, test add var to finsh);
FINSH_FUNCTION_EXPORT_ALIAS(macro_add_func, func, test add func to finsh);

/*使用函数的方式向finsh中添加函数和变量
 *使用的条件是在Rtconfig.h中定义RT_USING_HEAP
 */
/*int var2;
long func_add_func(void)
{
    rt_kprintf("func_add_func succeed\n");
    return 0;
}

const char *func_name = "func_add_func";
const char *var_name = "var2";
finsh_syscall_append(func_name, &func_add_func);
finsh_sysvar_append(var_name, finsh_type_int, &var2);*/

/*
 *使用函数向msh中添加函数
 */
int msh_add_func(int argc, char **argv)
{
    int i = 1;
    rt_kprintf("argv[0]:%s\n", argv[0]);
    while (argc > i) {
        rt_kprintf("argv[%d]:%s\n", i, argv[i]);
        i++;
    }
    return 0;
}
MSH_CMD_EXPORT(msh_add_func, test add func to msh);

