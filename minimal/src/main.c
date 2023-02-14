#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "myMath.h"

void main(void)
{
    int a,b = 0;
    
    a = 10;
    b = 20;

    while(1)
    {
        printk("Hello world!!!\n");
        
        if(!(IS_ENABLED(CONFIG_MYMATH)))
        {
            printk("MYMATH not enabled \n");
            return;
        }
        else
        {
            printk(" %d + %d = %d\n",a,b, sum(a,b));
            k_msleep(1000);
        }
    }
}