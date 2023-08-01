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

#define COMPANY_ID_CODE				(0x0059)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static unsigned char url_data[] = {0x17,'/','/','a','c','a','d','e','m','y','.',
									'n','o','r','d','i','c','s','e','m','i','.',
									'c','o','m'};


static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_NONE,
												800,
												801,
												NULL
											);

typedef struct adv_mfg_data{
	uint16_t company_code;		/* Company indentifier code */
	uint16_t number_press;		/* Number of times button 1 is pressed */
}adv_mfg_data_type;

static adv_mfg_data_type adv_mfg_data = {COMPANY_ID_CODE, 0x00};

/* Advertising package */
static const struct bt_data ad[] ={
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, (unsigned char*) &adv_mfg_data, sizeof(adv_mfg_data))
};

/* Scan request package */
static const struct bt_data sd[] ={
	BT_DATA(BT_DATA_URI, url_data, sizeof(url_data))
};


void button_changed(uint32_t button_state, uint32_t has_changed){
	if(has_changed & button_state & USR_BUTTON){
		adv_mfg_data.number_press +=1;
		LOG_INF("Button 1 pressed, incrementing the counter to %d",adv_mfg_data.number_press);
		bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	}
}

static int init_button(void){
	int err=dk_buttons_init(button_changed);
	if(err){
		LOG_ERR("Button handler init error: %d",err);
		return err;
	}
	return err;
}

void main(void)
{
	int err=dk_leds_init();
	int led_state=0;

	/* LED DK init */
	if(err){
		LOG_ERR("Led init error: %d",err);
		return;
	}

	/* Button DK init */
	err=init_button();
	if(err){
		LOG_ERR("Button init error: %d",err);
		return ;
	}

	/* BLE init */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("BT enable error:%d",err);
		return;
	}

	/* BLE advertising init */
	err=bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
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
