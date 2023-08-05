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

/* GPIO structures */
struct gpio_dt_spec system_led = GPIO_DT_SPEC_GET(DT_ALIAS(systemstatus), gpios);
struct gpio_dt_spec ble_led = GPIO_DT_SPEC_GET(DT_ALIAS(blestatus), gpios);
struct gpio_dt_spec dbg_button = GPIO_DT_SPEC_GET(DT_ALIAS(dbgbutton), gpios);


static int init_leds(void){
	/* Note: The LEDs are connected with a pull-up resistor, so to turn it off needs to
	set the GPIO as high state. */
	int err=0;

	/* Configure System status led as output without interrupt */
	err=gpio_pin_configure_dt(&system_led,(GPIO_OUTPUT_HIGH | GPIO_INT_DISABLE));
	if(err){
		LOG_ERR("Failed to configure the system status led, error:%d",err);
		return err;
	}
	
	if(!gpio_is_ready_dt(&system_led)){
		LOG_ERR("The system status led is not ready, error");
		return -1;
	}

	/* Configure ble connection status led as output without interrupt */
	err=gpio_pin_configure_dt(&ble_led,(GPIO_OUTPUT_HIGH | GPIO_INT_DISABLE));
	if(err){
		LOG_ERR("Failed to configure the ble status led, error:%d",err);
		return err;
	}

	if(!gpio_is_ready_dt(&ble_led)){
		LOG_ERR("The ble status led is not ready, error");
		return -1;
	}

	return err;
}

static int init_button(void){
	int err=0;
	err = gpio_pin_configure_dt(&dbg_button, (GPIO_INPUT | GPIO_INT_DISABLE));
	
	if(err){
		LOG_ERR("Failed to configure the dbg button, error:%d",err);
		return err;
	}
	
	if(!gpio_is_ready_dt(&dbg_button)){
		LOG_ERR("The dbg button is not ready, error");
		return -1;
	}

	return err;
}

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

	/* Init BLE */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("Failed to enable BLE, error %d",err);
		return;
	}

	/* Configure and start BLE advertising */
	int status_led=0;
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		gpio_pin_set(system_led.port, system_led.pin, (++status_led)%2);		
		k_msleep(STATUS_LED_TIME);
	}
}
