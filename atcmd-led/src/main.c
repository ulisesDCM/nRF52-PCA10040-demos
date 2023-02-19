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

const struct gpio_dt_spec btnList[4] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(btn1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(btn2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(btn3), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(btn4), gpios),
};

static int led_init(void)
{
    for(uint8_t i=0; i<4; i++)
    {
        if(!device_is_ready(ledList[i].port))
        {
            LOG_ERR("Error in the initialization of the LED %d",i);
            return 1;
        }

        gpio_pin_configure_dt(&ledList[i], (GPIO_OUTPUT | GPIO_PUSH_PULL));
    }
    return 0;
}

static int buttons_init(void)
{
    for(uint8_t i=0; i<4; i++)
    {
        if(!device_is_ready(btnList[i].port))
        {
            LOG_ERR("Error in the initialization of the Button %d",i);
            return 1;
        }

        gpio_pin_configure_dt(&btnList[i], (GPIO_INPUT | GPIO_PULL_UP));
    }

    return 0;
}

int main(void)
{
    if(led_init())
        return 1;   //Error

    if(buttons_init())
        return 1;   //Error
        
    while(1)
    {
        LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    
    return 1;
}