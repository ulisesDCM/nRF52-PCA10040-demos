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

#define STATUS_LED				(DK_LED1)
#define STATUS_LED_TIME			(1000)
#define BT_CONN_LED				(DK_LED2)


void main(void)
{
	int err=0;
	int status_led=0;
	/* init DK LED */
	err=dk_leds_init();
	if(err){
		LOG_ERR("Failed initialize DK LED, %d",err);
		return;
	}

	
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		k_msleep(STATUS_LED_TIME);
	}
}
