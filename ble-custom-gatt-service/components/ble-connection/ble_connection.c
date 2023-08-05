#include "ble_connection.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>

LOG_MODULE_REGISTER(ble_connection_c, LOG_LEVEL_DBG);

void ble_connection_test(void){
    LOG_INF("Hello ble_connection\n");
}