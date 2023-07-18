/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "zephyr/logging/log.h"

/* Thread basic configuration */
#define STACK_SIZE			(1024*1)
#define THREAD0_PRIORITY	(2)
#define THREAD1_PRIORIY		(3)
#define WORKQ_PRIORITY		(4)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* Thread functions */
void thread0(void){
	uint64_t time_stamp;
	int64_t delta_time;
	int64_t counter=0;
	while(1){
		time_stamp=k_uptime_get();
		/* Emulate work */
		for(uint32_t i=0;i<400000;i++){
			counter+=counter;
		}
		delta_time=k_uptime_delta(&time_stamp);

		LOG_INF("Thread0 yielding this round in %lld ms",delta_time);
		k_msleep(20);
		// k_busy_wait(1000000);
		// k_yield();
	}
}

void thread1(void){
	int64_t time_stamp;
	int64_t counter=0;
	int64_t delta_time;
	while(1){
		time_stamp=k_uptime_get();
		/* Emulate work */
		for(uint32_t i=0;i<400000;i++){
			counter+=counter;
		}
		delta_time = k_uptime_delta(&time_stamp);
		LOG_INF("Thread1 yielding this round in %lld ms",delta_time);
		k_msleep(20);
		// k_busy_wait(1000000);
		// k_yield();
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
