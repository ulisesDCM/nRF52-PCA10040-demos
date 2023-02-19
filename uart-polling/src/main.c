#include <stdio.h>
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"

#include "zephyr/drivers/uart.h"

// LOG_MODULE_DECLARE(main, LOG_LEVEL_DBG);
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

const struct uart_config uart0_config = {
    .baudrate=115200,
    .data_bits=UART_CFG_DATA_BITS_8,
    .flow_ctrl=UART_CFG_FLOW_CTRL_NONE,
    .parity=UART_CFG_PARITY_NONE,
    .stop_bits=UART_CFG_STOP_BITS_1
};

static void uart_send_data(uint8_t *str)
{
    while(*str)
        uart_poll_out(uart, *str++);
}

static void uart_get_data(uint8_t *str)
{
    do
    {

    }while(*str);
}

int main(void)
{
    if(!device_is_ready(uart))
    {
        LOG_ERR("Error in the UART0 initialization");
        return 1;
    }

    int err = uart_configure(uart, &uart0_config);
    if (err == -ENOSYS)
    {
        LOG_ERR("Error configuring the UART0.");
        return -ENOSYS;
    }

    unsigned char data_t[] = "Hello world using UART0 \r\n";     //Data to transmitt.
    unsigned char data_r[20] = {0};                     //Data to receive.

    LOG_INF("Hello world using RTT");

    while(1)
    {
        uart_send_data(data_t);             //Transmitt data

        // while(data_r[0] == 0)
        //     uart_poll_in(uart, &data_r[0]);

        // if(data_r[0] != 0)
        // {
        //     uart_poll_out(uart, data_r[0]);
        //     data_r[0] = 0;
        // }
        
        k_msleep(1000);
    }
    return 1;
}