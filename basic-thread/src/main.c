/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "zephyr/logging/log.h"

/* Thread basic configuration */
#define STACK_SIZE			(1024*1)
#define THREAD0_PRIORITY	(6)
#define THREAD1_PRIORIY		(6)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* Thread functions */
void thread0(void){
	while(1){
		LOG_INF("Hello, I am thread0 :)");
		k_msleep(5);
		// k_yield();
	}
}

void thread1(void){
	k_msleep(10);
	while(1){
		LOG_INF("Hello, I am thread1 ;)");
		// k_yield();
		k_msleep(5);
	}
}


K_THREAD_DEFINE(thread0_id, STACK_SIZE, thread0, NULL, NULL, NULL, 
					THREAD0_PRIORITY, 0, 0);
	
K_THREAD_DEFINE(thread1_id, STACK_SIZE, thread1, NULL, NULL, NULL, 
					THREAD1_PRIORIY, 0, 0);

/* Main thread loop */
// void main(void)
// {
// 	LOG_INF("Hello World %s",CONFIG_BOARD);

// 	/* Thread definitions */
// }
