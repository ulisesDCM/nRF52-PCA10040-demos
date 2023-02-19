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

const struct uart_config uartConfig = {
    .baudrate=115200,
    .data_bits=UART_CFG_DATA_BITS_8,
    .flow_ctrl=UART_CFG_FLOW_CTRL_NONE,
    .parity=UART_CFG_PARITY_NONE,
    .stop_bits=UART_CFG_STOP_BITS_1
};

const struct device *uartDev = DEVICE_DT_GET(DT_NODELABEL(uart0));

void serial_comm_cb(const struct device *dev, 
                    struct uart_event *evt,
                    void *user_data)
{
    switch (evt->type)
    {
    case UART_TX_DONE:
        break;

    case UART_TX_ABORTED:
        break;

    case UART_RX_RDY:
        break;

    case UART_RX_BUF_REQUEST:
        break;

    case UART_RX_BUF_RELEASED:
        break;

    case UART_RX_DISABLED:
        break;

    case UART_RX_STOPPED:
        break;
    }
}

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

static int serial_comm_init(void)
{
    if(!device_is_ready(uartDev))
    {
        LOG_ERR("Error in the initialization of the UART0");
        return 1;
    }

    if( uart_configure(uartDev, &uartConfig) < 0)
    {
        LOG_ERR("Error in the configuration of the UART0");
        return 1;
    }

    if( uart_callback_set(uartDev, serial_comm_cb, NULL) < 0)
    {
        LOG_ERR("Error setting the UART callback function.");
        return 1;
    }
    return 0;
}

int main(void)
{
    if(led_init())
        return 1;   //Error

    if(buttons_init())
        return 1;   //Error

    if(serial_comm_init())
        return 1;

    while(1)
    {
        k_msleep(1000);
    }
    
    return 1;
}