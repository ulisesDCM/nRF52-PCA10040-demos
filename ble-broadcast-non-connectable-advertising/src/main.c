/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#define DEVICE_NAME					(CONFIG_BT_DEVICE_NAME)
#define DEVICE_NAME_LEN				(sizeof(DEVICE_NAME)-1)

#define RUN_STATUS_LED				(DK_LED1)
#define RUN_LED_BLINK_TERMINAL		(1000)


LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* Defining the URL for send as a scan response */
static unsigned char url_data[] = {0x17,'/','/','a','c','a','d','e','m','y','.',
									'n','o','r','d','i','c','s','e','m','i','.',
									'c','o','m'};

/* the advertising packet to send. */
static const struct  bt_data ad[]={
	/* Advertising flags */
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),				
	/* Advertising packet data */
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),	
};

/* scan response  */
static const struct bt_data sd[]={
	BT_DATA(BT_DATA_URI, url_data, sizeof(url_data))	
};

void main(void)
{
	
	int blink_status=0;
	int err;
	err=dk_leds_init();
	if(err){
		LOG_ERR("LEDs init failed (err %d)",err);
		return;
	}

	/*Init BLE  */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("BLE init failed (err %d)",err);
		return;	
	}
	LOG_INF("Bluetooth initialized!!!");

	/* Init BLE advertising mode */
	err=bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if(err){
		LOG_ERR("BLE Advertising failed to start (err %d)\n",err);
		return;
	}

	



	LOG_INF("Advertising succesfully started");
	while(1){
		dk_set_led(RUN_STATUS_LED, (++blink_status)%2);
		k_msleep(RUN_LED_BLINK_TERMINAL);
	}
}
