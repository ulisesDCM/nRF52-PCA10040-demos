#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(gpio_interrupt, LOG_LEVEL_INF);

#define BTN1_NODE   DT_ALIAS(btn1)
#define BTN2_NODE   DT_ALIAS(btn2)
#define BTN3_NODE   DT_ALIAS(btn3)
#define BTN4_NODE   DT_ALIAS(btn4)

#define LED1_NODE   DT_ALIAS(ld1)
#define LED2_NODE   DT_ALIAS(ld2)
#define LED3_NODE   DT_ALIAS(ld3)
#define LED4_NODE   DT_ALIAS(ld4)


static const struct gpio_dt_spec ledList[4] = {
                GPIO_DT_SPEC_GET(LED1_NODE, gpios),
                GPIO_DT_SPEC_GET(LED2_NODE, gpios),
                GPIO_DT_SPEC_GET(LED3_NODE, gpios),
                GPIO_DT_SPEC_GET(LED4_NODE, gpios)
                };

static const struct gpio_dt_spec buttonsLit[4] = {
                GPIO_DT_SPEC_GET(BTN1_NODE, gpios),
                GPIO_DT_SPEC_GET(BTN2_NODE, gpios),
                GPIO_DT_SPEC_GET(BTN3_NODE, gpios),
                GPIO_DT_SPEC_GET(BTN4_NODE, gpios)
};

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