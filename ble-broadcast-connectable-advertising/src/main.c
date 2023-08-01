#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "dk_buttons_and_leds.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#define DEVICE_NAME				(CONFIG_BT_DEVICE_NAME)
#define DEVICE_NAME_LEN			(sizeof(DEVICE_NAME)-1)
#define USER_LED				(DK_LED1)
#define USER_LED_TIME			(1000)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

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

	/* BLE init */
	
	/* BLE advertising init */

	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(USER_LED, (++led_status)%2);
		k_msleep(USER_LED_TIME);
	}
}
