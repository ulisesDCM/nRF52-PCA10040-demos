#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gpio_polling_exer, LOG_LEVEL_DBG);

int main(void)
{
    while(1)
    {
        LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    return 1;
}
