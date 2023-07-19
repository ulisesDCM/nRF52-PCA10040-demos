/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/rand32.h>

#define THREAD0_PRIORITY		(4)
#define THREAD1_PRIORITY		(4)
#define COMBINED_TOTAL			(40)

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

int32_t increment_count	=0;
int32_t decrement_count = COMBINED_TOTAL;

K_MUTEX_DEFINE(test_mutex);

void shared_code_section(void){
	k_mutex_lock(&test_mutex, K_FOREVER);

	increment_count+=1;
	increment_count=increment_count%COMBINED_TOTAL;
	decrement_count-=1;

	if(decrement_count==0){
		decrement_count=COMBINED_TOTAL;
	}

	k_mutex_unlock(&test_mutex);

	if(increment_count+decrement_count != COMBINED_TOTAL){
		LOG_INF("Race condition happend!!!");
		LOG_INF("increment_count(%d) + decrement_count(%d) = %d",increment_count,
																decrement_count,
															(increment_count+decrement_count));
		k_msleep(400+sys_rand32_get()%10);
	}
}

void thread0(void){
	LOG_INF("Thread 0 started");
	while(1){
		shared_code_section();
	}
}

void thread1(void){
	LOG_INF("Thread 1 started");
	while(1){
		shared_code_section();
	}
}

K_THREAD_DEFINE(thread_0, 1024*2, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 5000);
K_THREAD_DEFINE(thread_1, 1024*2, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 5000);

void main(void)
{
	LOG_INF("Hello world!!! %s",CONFIG_BOARD);
}
