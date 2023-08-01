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

#define USR_BUTTON 					(DK_BTN1_MSK)

#define DEVICE_NAME					(CONFIG_BT_DEVICE_NAME)
#define DEVICE_NAME_LEN				(sizeof(DEVICE_NAME)-1)

#define RUN_STATUS_LED				(DK_LED1)
#define RUN_LED_BLINK_INTERVAL		(1000)


LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static unsigned char url_data[] = {0x17,'/','/','a','c','a','d','e','m','y','.',
									'n','o','r','d','i','c','s','e','m','i','.',
									'c','o','m'};

/* Advertising package */
static const struct bt_data ad[] ={
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

/* Scan request package */
static const struct bt_data sd[] ={
	BT_DATA(BT_DATA_URI, url_data, sizeof(url_data))
};

void main(void)
{
	int err=dk_leds_init();
	int led_state=0;

	if(err){
		LOG_ERR("Led init error: %d",err);
		return;
	}

	/* BLE init */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("BT enable error:%d",err);
		return;
	}

	/* BLE advertising init */
	err=bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if(err){
		LOG_ERR("BT Adv error:%d",err);
		return;
	}
	
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(DK_LED1, (++led_state)%2);
		k_msleep(1000);
	}
}
