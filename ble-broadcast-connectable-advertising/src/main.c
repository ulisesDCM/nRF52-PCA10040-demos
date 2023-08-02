#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "dk_buttons_and_leds.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>

#define DEVICE_NAME				(CONFIG_BT_DEVICE_NAME)
#define DEVICE_NAME_LEN			(sizeof(DEVICE_NAME)-1)
#define USER_LED				(DK_LED1)
#define USER_LED_TIME			(1000)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, 
					BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123))
};

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM( (BT_LE_ADV_OPT_CONNECTABLE | 
															BT_LE_ADV_OPT_USE_IDENTITY),
										800,	/* Min advertising interval 500ms (800*0.625ms) */
										801,	/* Max advertising interval 500.625ms (801*0.625ms) */
										NULL	/* Set to NULL for undirected advertising */
										);
						
void main(void)
{
	int led_status=0;
	int err=0;

	/* DK LEDs init */
	err=dk_leds_init();
	if(err){
		LOG_ERR("DK LEDs init error:%d",err);
		return ;
	}

	/* Setting the static random BLE address */
	bt_addr_le_t addr;
	err=bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
	if(err){
		LOG_ERR("Invalid BT address, error: %d",err);
		return;
	}
	
	err=bt_id_create(&addr, NULL);
	if(err){
		LOG_ERR("Error creating the static random address: %d",err);
		return;
	}

	/* BLE init */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("Error enabling bluetooth : %d",err);
		return ;
	}	

	err=bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
	if(err){
		LOG_ERR("Advertising failed to start: %d",err);
		return;
	}

	/* BLE advertising init */

	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(USER_LED, (++led_status)%2);
		k_msleep(USER_LED_TIME);
	}
}
