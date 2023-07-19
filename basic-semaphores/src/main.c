/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/rand32.h>
#include <zephyr/kernel.h>


#define PRODUCER_PRIORITY			(4)
#define CONSUMER_PRIORITY			(5)
#define PRODUCER_STACK_SIZE			(1024*2)
#define CONSUMER_STACK_SIZE			(1024*2)

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

volatile uint32_t available_instance_count=10;

void release_access(void){
	available_instance_count++;
	LOG_INF("Resource given and available_instance_count = %d",available_instance_count);

}

void get_access(void){
	available_instance_count--;
	LOG_INF("Resource taken and available_instance_count = %d",available_instance_count);
}

void producer(void){
	LOG_INF("Producer thread started!!!");
	while(1){
		release_access();
		k_msleep(500+sys_rand32_get()%10);
	}
}


void consumer(void){
	LOG_INF("Consumer thread started!!!");
	while(1){
		get_access();
		k_msleep(sys_rand32_get()%10);
	}
}

K_THREAD_DEFINE(consumer_thread, CONSUMER_STACK_SIZE, consumer, 
						NULL,NULL,NULL,CONSUMER_PRIORITY, 0, 0);

K_THREAD_DEFINE(producer_thread, PRODUCER_STACK_SIZE, producer, 
						NULL,NULL,NULL,PRODUCER_PRIORITY, 0, 0);

void main(void)
{
	LOG_INF("Hello World %s!!!",CONFIG_BOARD);
}
