#include <stdio.h>

#include <zephyr/zephyr.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    while(1)
    {
        LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    
    return 1;
}