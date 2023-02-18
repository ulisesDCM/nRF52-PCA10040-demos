#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(gpio_interrupt, LOG_LEVEL_INF);

int main(void)
{
    while(1)
    {
        LOG_INF("Hello world!!!");
        LOG_ERR("Error ");
        k_msleep(1000);
    }
    return 1;
}