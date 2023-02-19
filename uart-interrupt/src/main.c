#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/uart.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

const struct uart_config uartConf = {
    .baudrate=115200,
    .data_bits=UART_CFG_DATA_BITS_8,
    .flow_ctrl=UART_CFG_FLOW_CTRL_NONE,
    .parity=UART_CFG_PARITY_NONE,
    .stop_bits=UART_CFG_STOP_BITS_1
};


static void uart0_cb(const struct device *dev, 
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

int main(void)
{
    if(!device_is_ready(uart))
    {
        LOG_ERR("Error in initialization of the UART0 port.");
        return 1;
    }

    if(uart_configure(uart, &uartConf) < 0)
    {
        LOG_ERR("Error configuring the UART0 port.");
        return 1;
    }

    if(uart_callback_set(uart, uart0_cb, NULL) < 0)
    {
        LOG_ERR("Error setting the UART0 callback function.");
        return 1;
    }
    
    while(1)
    {
            LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    
    return 1;
}