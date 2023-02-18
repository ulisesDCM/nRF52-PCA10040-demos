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

static struct gpio_callback btn1_cb_data;

void btn1_isr(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t pins)
{
    LOG_INF("Executing button1 isr function.");   
    for(uint8_t i=0; i<4; i++)
    {
        gpio_pin_toggle_dt(&ledList[i]);
    }
}

static int init_gpios(void)
{
    //Init LEDs
    LOG_INF("Starting the initialization of the list of LEDs");
    for(uint8_t i=0; i<4; i++)
    {
        if(!device_is_ready(ledList[i].port))
        {
            LOG_ERR("Initialization error in the led %d. ",(i+1));
            return 0;   //Return error.
        }
        gpio_pin_configure_dt(&ledList[i], (GPIO_OUTPUT | GPIO_PUSH_PULL));    
    }
    LOG_INF("The list of LEDs was initialize correctly.");
    //Init Buttons.
    LOG_INF("Starting the initialization of the list of buttons.");
    for(uint8_t i=0; i<4; i++)
    {
        if(!device_is_ready(buttonsLit[i].port))
        {
            LOG_ERR("Initialization error in the led %d.",(i+1));
        }
        gpio_pin_configure_dt(&buttonsLit[i], (GPIO_INPUT | GPIO_PULL_UP));

        
    
    }  
        //Start interrupt configuration for the buttons.
        gpio_pin_interrupt_configure_dt(&buttonsLit[0], GPIO_INT_EDGE_FALLING);

        //Defining the callback function.
        gpio_init_callback(&btn1_cb_data, btn1_isr, BIT(buttonsLit[0].pin));

        gpio_add_callback(buttonsLit[0].port, &btn1_cb_data);

    LOG_INF("The list of buttons was initialize correctly.");

    return 1;
}

int main(void)
{
    if(!init_gpios())
        return 0;

    while(1)
    {
        k_msleep(1000);
    }
    return 1;
}