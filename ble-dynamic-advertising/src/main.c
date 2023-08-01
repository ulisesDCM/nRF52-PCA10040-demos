/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#include "dk_buttons_and_leds.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void main(void)
{
	int err=dk_leds_init();
	int led_state=0;

	if(err){
		LOG_ERR("Led init error: %d",err);
		return;
	}

	/* BLE init */
	err=bt_enable(NULL)
	if(err){
		LOG_ERR("BT enable error:%d",err);
		return;
	}

	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(DK_LED1, (++led_state)%2);
		k_msleep(1000);
	}
}
