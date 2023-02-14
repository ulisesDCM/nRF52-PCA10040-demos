#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* 
    The output of the printk() is not deferred, meaning the output is 
    sent immediately to the console without any mutual exclusion or 
    buffering.
    Logs are sent immediately as they are issued, and printk() 
    will not return until all bytes of the message are sent
*/
int main(void)
{
    while(1)
    {
        printk("Hello world!!!\n");
        k_msleep(1000);
    }
    return 0;
}