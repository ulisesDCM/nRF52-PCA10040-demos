#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "dk_buttons_and_leds.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define LED_STATUS				(DK_LED1)
#define LED_STATUS_TIME			(1000)
#define BT_DEVICE_NAME			(CONFIG_BT_DEVICE_NAME)
#define BT_DEVICE_NAME_LEN		(sizeof(BT_DEVICE_NAME)-1)


void main(void)
{	
	int err=0;
	int led_status=0;
	
	/* LED dk configuration */
	err=dk_leds_init();
	if(err){
		LOG_ERR("Failed to configure leds, error:%d",err);
		return;
	}

	/* BLE init */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("Failed to init BLE, error:%d",err);
		return;
	}

	/* Main loop */
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(LED_STATUS, (++led_status)%2);
		k_msleep(1000);
	}
}
