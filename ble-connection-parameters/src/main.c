/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define STATUS_LED_TIME			(1000)


void main(void)
{
	int err=0;
	int status_led=0;

	
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);		
		k_msleep(STATUS_LED_TIME);
	}
}
