#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/gpio.h>

#define PCA100_BOARD_LEDS   (4)
#define PCA100_BOARD_BTNS   (4)

// Log module configuration.
LOG_MODULE_REGISTER(gpio_polling_exer, LOG_LEVEL_DBG);

// Array of LEDs.
struct gpio_dt_spec usr_leds[PCA100_BOARD_LEDS] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(usrled1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrled2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrled3), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrled4), gpios)
};

// Array of buttons.
struct gpio_dt_spec usr_buttons[PCA100_BOARD_BTNS] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(usrbtn1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrbtn2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrbtn3), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(usrbtn4), gpios),
};


static int leds_init(void)
{
    for(uint8_t i=0; i<PCA100_BOARD_LEDS; i++)
    {
        // Wait for ready.
        if(!device_is_ready( usr_leds[i].port))
        {
            LOG_ERR("Error in the initialization of the LED %d",i+1);
            return 0;
        }

        // Configure as output.
        gpio_pin_configure_dt((const struct gpio_dt_spec *) &usr_leds[i], 
                                (GPIO_OUTPUT | GPIO_ACTIVE_LOW));
    }

    
    return 1;
}

static int buttons_init(void)
{
    for(uint8_t i=0; i<PCA100_BOARD_BTNS; i++)
    {
        // Wait for ready.
        if(!device_is_ready( usr_buttons[i].port))
        {
            LOG_ERR("Error in the initialization of the BUTTON %d",i+1);
            return 0;
        }

        // Configure as input with pull-up resistor.
        gpio_pin_configure_dt( (const struct gpio_dt_spec *) &usr_buttons[i], 
                                (GPIO_INPUT | GPIO_PULL_UP) );
    }
    return 1;
}


int main(void)
{
    //Initalize all LEDs.
    if(!leds_init())
        return 0;

    //Intialize all buttons.
    if(!buttons_init())
        return 0;

    while(1)
    {
        for(uint8_t i=0; i<PCA100_BOARD_LEDS; i++)
        {
            LOG_INF("Toogling the LED %d.",i);
            gpio_pin_toggle_dt( (const struct gpio_dt_spec *) &usr_leds[i]);
            k_msleep(1000);
        }    
    }
    return 1;
}
