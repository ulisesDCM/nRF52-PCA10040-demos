#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("Hello world!!!\n");
    return 0;
}