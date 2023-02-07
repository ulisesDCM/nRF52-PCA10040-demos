/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* Getting all nodes from the device Tree overlay file. */

/* Button nodes. */
#define BTN1_NODE		DT_NODELABEL(button0)
#define BTN2_NODE		DT_NODELABEL(button1)
#define BTN3_NODE		DT_NODELABEL(button2)
#define BTN4_NODE		DT_NODELABEL(button3)

/* LED Nodes. */
#define LED0_NODE 		DT_NODELABEL(led0)
#define LED1_NODE 		DT_NODELABEL(led1)
#define LED2_NODE 		DT_NODELABEL(led2)
#define LED3_NODE 		DT_NODELABEL(led3)

/* User buttons device structures. */
static const struct gpio_dt_spec usr_btn1 = GPIO_DT_SPEC_GET(BTN1_NODE, gpios);
static const struct gpio_dt_spec usr_btn2 = GPIO_DT_SPEC_GET(BTN2_NODE, gpios);
static const struct gpio_dt_spec usr_btn3 = GPIO_DT_SPEC_GET(BTN3_NODE, gpios);
static const struct gpio_dt_spec usr_btn4 = GPIO_DT_SPEC_GET(BTN4_NODE, gpios);

/* User LED device structures. */
static const struct gpio_dt_spec usr_led1 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec usr_led2 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec usr_led3 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec usr_led4 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

static void system_error(void);
static int system_init(void);

void main(void)
{
	int system_status = 0;
	
	/* Initialize system */
	system_status = system_init();

	if(!system_status)
		system_error();


	/* Main while loop */
	while(1)
	{	
		k_msleep(1000);
	}
}

static void system_error(void)
{
	while(1);
}

static int system_init(void)
{
	/* Check buttons ports */
	if(!device_is_ready(usr_btn1.port))
		return false;

	if(!device_is_ready(usr_btn2.port))
		return false;

	if(!device_is_ready(usr_btn3.port))
		return false;

	if(!device_is_ready(usr_btn4.port))
		return false;

	/* Check LEDs ports */
	if(!device_is_ready(usr_led1.port))
		return false;

	if(!device_is_ready(usr_led2.port))
		return false;

	if(!device_is_ready(usr_led3.port))
		return false;

	if(!device_is_ready(usr_led4.port))
		return false;

	/* Configure the buttons */
	if( gpio_pin_configure_dt(&usr_btn1, GPIO_INPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_btn2, GPIO_INPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_btn3, GPIO_INPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_btn4, GPIO_INPUT) != 0)
		return false;

	/* Configure the LEDs */
	if( gpio_pin_configure_dt(&usr_led1, GPIO_OUTPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_led2, GPIO_OUTPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_led3, GPIO_OUTPUT) != 0)
		return false;

	if( gpio_pin_configure_dt(&usr_led4, GPIO_OUTPUT) != 0)
		return false;

}