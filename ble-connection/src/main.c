#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "dk_buttons_and_leds.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>
#include <bluetooth/services/lbs.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define LED_STATUS					(DK_LED1)
#define LED_CONNECTION_STATUS		(DK_LED2)
#define LED_STATUS_TIME				(1000)
#define USR_BUTTON					(DK_BTN1_MSK)
#define BT_DEVICE_NAME				(CONFIG_BT_DEVICE_NAME)
#define BT_DEVICE_NAME_LEN			(sizeof(BT_DEVICE_NAME)-1)


struct bt_conn *my_conn=NULL;

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM( (BT_LE_ADV_OPT_CONNECTABLE |
															BT_LE_ADV_OPT_USE_IDENTITY),
														BT_GAP_ADV_FAST_INT_MIN_1,
														BT_GAP_ADV_FAST_INT_MAX_1,
														NULL);
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, BT_DEVICE_NAME, BT_DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 
															0x1212, 
															0xefde, 
															0x1523, 
															0x785feabcd123
															)),

};

void on_connected(struct bt_conn *conn, uint8_t err){
	if(err){
		LOG_ERR("Connection error: %d",err);
		return;
	}
	LOG_INF("Connected");
	my_conn = bt_conn_ref(conn);

	/* Turn on the connection status LED */
	dk_set_led_on(LED_CONNECTION_STATUS);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason){
	/* The BT error codes are in the hci_err.h */
	LOG_ERR("Diconnect. reason %d",reason);
	bt_conn_unref(conn);

	/* Turn off the connection status LED */
	dk_set_led_off(LED_CONNECTION_STATUS);
}

struct bt_conn_cb connection_callbacks = {
	.connected=on_connected,
	.disconnected=on_disconnected
};

void button_handler(uint32_t button_state, uint32_t has_changed){
	int err=0;
	if(has_changed & USR_BUTTON){
		/* USR_BUTTON was pressed */
		LOG_INF("Button changed");
		err=bt_lbs_send_button_state(button_state ? true : false);
		if(err){
			LOG_ERR("Couldn't send notification. err:%d",err);
		}
	}
}

static int init_button(void){
	int err=0;
	err=dk_buttons_init(button_handler);
	if(err){
		LOG_ERR("Failed to init DK button, err:%d",err);
	}

	return err;
}

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

	/* Buttons DK init  */
	err=init_button();
	if(err){
		LOG_ERR("Failed to configure button, error:%d",err);
		return;
	}

	/* Register the connect/disconnect callbacks */
	bt_conn_cb_register(&connection_callbacks);

	/* BLE init */
	err=bt_enable(NULL);
	if(err){
		LOG_ERR("Failed to init BLE, error:%d",err);
		return;
	}

	/* BLE start advertising */
	err=bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd) );
	if(err){
		LOG_ERR("Failed to start advertising, error:%d",err);
		return;
	}

	/* Main loop */
	while(1){
		LOG_INF("Hello World! %s\n", CONFIG_BOARD);
		dk_set_led(LED_STATUS, (++led_status)%2);
		k_msleep(1000);
	}
}
