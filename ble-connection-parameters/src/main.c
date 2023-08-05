/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>
#include <bluetooth/services/lbs.h>

/* LOG module register */
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* System constants */
#define STATUS_LED_TIME			(1000)
#define DEVICE_NAME				(CONFIG_BT_DEVICE_NAME)
#define DEVICE_NAME_LEN			(sizeof(DEVICE_NAME)-1)

/* GPIO structures */
struct gpio_dt_spec system_led = GPIO_DT_SPEC_GET(DT_ALIAS(systemstatus), gpios);
struct gpio_dt_spec ble_led = GPIO_DT_SPEC_GET(DT_ALIAS(blestatus), gpios);
struct gpio_dt_spec dbg_button = GPIO_DT_SPEC_GET(DT_ALIAS(dbgbutton), gpios);

/* Debug button callback */
static struct gpio_callback dbg_btn_cb;

/* Function prototypes */
static int init_leds(void);
static int init_button(void);

/* Bluetooth adv param 
	* Connectable advertising and use identity address 
	* 0x30 units, 48 units, 30ms.
	* 0x60 units, 96 units, 60ms
	* Set to NULL for undirected advertising
*/
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(( BT_LE_ADV_OPT_CONNECTABLE |
																BT_LE_ADV_OPT_USE_IDENTITY),
														BT_GAP_ADV_FAST_INT_MIN_1,
														BT_GAP_ADV_FAST_INT_MAX_1,
														NULL);
struct bt_conn *my_conn = NULL;

static const struct bt_data ad[] ={
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, 
					BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123))
					
};

void dbg_btn_callback(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins){
	int err=0;
	LOG_INF("Button state is: %d",gpio_pin_get(port,dbg_button.pin));
	err = bt_lbs_send_button_state(gpio_pin_get(port,dbg_button.pin));
	if(err){
		LOG_ERR("Couldn't send notification. err:%d",err);
	}
}

void on_connected(struct bt_conn *conn, uint8_t err){
	if(err){
		LOG_ERR("Connection error %d",err);
		return;
	}
	LOG_INF("Connected");
	my_conn = bt_conn_ref(conn);
	gpio_pin_set(ble_led.port, ble_led.pin, true);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason){
	LOG_INF("Disconnected. Reason %d",reason);
	gpio_pin_set(ble_led.port, ble_led.pin, false);
	bt_conn_unref(my_conn);
}


struct bt_conn_cb bt_connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected
};

void main(void)
{
	int err=0;

	/* Init LEDs */
	err=init_leds();
	if(err){
		LOG_ERR("Failed to initialized leds, error: %d",err);
		return;
	}	

	/* Init buttons */
	err=init_button();
	if(err){
		LOG_ERR("Failed to initialized button, error: %d",err);
		return;
	}

	/* Configure static random BLE UUID  */
	bt_conn_cb_register(&bt_connection_callbacks);

	/* Init BLE */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("Failed to enable BLE, error %d",err);
		return;
	}

	/* Configure and start BLE advertising */
	err=bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if(err){
		LOG_ERR("Advertising failed to start (err %d)",err);
		return ;
	}

	int status_led=0;
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		gpio_pin_set(system_led.port, system_led.pin, (++status_led)%2);		
		k_msleep(STATUS_LED_TIME);
	}
}

static int init_leds(void){
	/* Note: The LEDs are connected with a pull-up resistor, so to turn it off needs to
	set the GPIO as high state. */
	int err=0;

	/* Configure System status led as output without interrupt */
	if(!gpio_is_ready_dt(&system_led)){
		LOG_ERR("The system status led is not ready, error");
		return -1;
	}

	err=gpio_pin_configure_dt(&system_led,(GPIO_OUTPUT_HIGH | GPIO_INT_DISABLE));
	if(err){
		LOG_ERR("Failed to configure the system status led, error:%d",err);
		return err;
	}

	/* Configure ble connection status led as output without interrupt */
	if(!gpio_is_ready_dt(&ble_led)){
		LOG_ERR("The ble status led is not ready, error");
		return -1;
	}

	err=gpio_pin_configure_dt(&ble_led,(GPIO_OUTPUT_HIGH | GPIO_INT_DISABLE));
	if(err){
		LOG_ERR("Failed to configure the ble status led, error:%d",err);
		return err;
	}


	return err;
}

static int init_button(void){
	int err=0;
	
	if(!gpio_is_ready_dt(&dbg_button)){
		LOG_ERR("The dbg button is not ready, error");
		return -1;
	}

	err = gpio_pin_configure_dt(&dbg_button, (GPIO_INPUT | GPIO_PULL_UP));
	if(err){
		LOG_ERR("Failed to configure the dbg button, error:%d",err);
		return err;
	}
	
	err = gpio_pin_interrupt_configure_dt(&dbg_button, GPIO_INT_EDGE_BOTH);
	gpio_init_callback(&dbg_btn_cb, dbg_btn_callback, BIT(dbg_button.pin));
	err = gpio_add_callback(dbg_button.port, &dbg_btn_cb);
	if(err){
		LOG_ERR("Failed to configure button interrupt, error:%d",err);
		return err;
	}
	
	return err;
}
