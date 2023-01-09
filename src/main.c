/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <device.h>
#include <sys/printk.h>
#include <string.h>
#include <stdio.h>
#include "SPI_sample.h"
#include "I2C_sample.h"
#include "UART_sample.h"
#include "FifoHandler.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* threads periodicity in ms*/
#define THREAD_TEMP_PERIODICITY 2000
#define THREAD_ACC_PERIODICITY 1000
#define THREAD_MODEM_PERIODICITY 4000
#define THREAD_FIFO_HANDLER_PERIODICITY 50

/* callback handler for messages coming from virtual acclerometer through SPI */
void readAccSPI(struct messageFrame *msg)
{
	printf("ACC READING = %s\n", msg->payload);
}
/* callback handler for messages coming from virtual temperature sensor through I2C */
void readTempI2C(struct messageFrame *msg)
{
	printf("TEMP READING = %s\n", msg->payload);
}
/* callback handler for messages coming from virtual modem through UART */
void readModemUART(struct messageFrame *msg)
{
	printf("MODEM RESPONSE = %s\n", msg->payload);
}

void main()
{
	/* assigning handlers for different type of messages */
	if (fifo_register_callback(&readTempI2C, MESS_TYPE_A) != ERR_OK)
		printk("callback A assignation failed\n");
	if (fifo_register_callback(&readAccSPI, MESS_TYPE_B) != ERR_OK)
		printk("callback B assignation failed\n");
	if (fifo_register_callback(&readModemUART, MESS_TYPE_C) != ERR_OK)
		printk("callback C assignation failed\n");
}

/* a thread where we get temperature readings from senor on the I2C bus */
void I2C_ConsumerThread(void)
{
	while (1) {
		/* simulation function just for messages subunit testing */
		task_temp_push();
		/* thread periodicity */
		k_msleep(THREAD_TEMP_PERIODICITY);
	}
}

/* same as I2C consumers but an acclerometer simulation through SPI */
void SPI_ConsumerThread(void)
{
	while (1) {
		task_acc_push();
		/* thread periodicity */
		k_msleep(THREAD_ACC_PERIODICITY);
	}
}

/* same as previous consumers but a modem simulation through UART */
void UART_ConsumerThread(void)
{
	while (1) {
		task_uart_push();
		/* thread periodicity */
		k_msleep(THREAD_MODEM_PERIODICITY);
	}
}

/* managing the fifo through that thread, dispatching messages and calling handlers that correspond to their message types */
void fifo_HandlerThread(void)
{
	while (1) {
		if (task_manage_msg() == ERR_INVALID_TYPE)
			printk("Message dispatcher fault!\n");
		/* thread periodicity */
		k_msleep(THREAD_FIFO_HANDLER_PERIODICITY);
	}
}

/* define and intialize 4 threads, 3 are demo threads to push simulated messages and the fifo handler thread */
/* the fifo handler thread */
K_THREAD_DEFINE(fifo_HandlerThread_id, STACKSIZE, fifo_HandlerThread, NULL, NULL, NULL, PRIORITY, 0,
		0);
K_THREAD_DEFINE(I2C_ConsumerThread_id, STACKSIZE, I2C_ConsumerThread, NULL, NULL, NULL, PRIORITY, 0,
		0);
K_THREAD_DEFINE(SPI_ConsumerThread_id, STACKSIZE, SPI_ConsumerThread, NULL, NULL, NULL, PRIORITY, 0,
		0);
K_THREAD_DEFINE(UART_ConsumerThread_id, STACKSIZE, UART_ConsumerThread, NULL, NULL, NULL, PRIORITY,
		0, 0);
