#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

const struct gpio_dt_spec ledList[4] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(ld1),gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(ld2),gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(ld3),gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(ld4),gpios),
};

static int led_init(void)
{
    for(uint8_t i=0; i<4; i++)
    {
        if(!device_is_ready(ledList[0].port))
        {
            LOG_ERR("Error in the initialization of the LED %d",i);
            return 1;
        }

        gpio_pin_configure_dt(&ledList[i], (GPIO_OUTPUT | GPIO_PUSH_PULL));
    }
    return 0;
}

int main(void)
{
    if(led_init())
    {
        return 1;
    }

    while(1)
    {
        LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    
    return 1;
}