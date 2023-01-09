#ifndef FIFO_HANDLER_H
#define FIFO_HANDLER_H

#include <string.h>

/* max number of message frame the fifo will handle, only FIFO_MAX_SIZE-1 can be actually pulled  */
#define FIFO_MAX_SIZE 100
/* the size of longest string the might be used as a payload in the message frame */
#define MAX_PAYLOAD_LEN 50

#define MAX_NUM_OF_MSG_TYPES 10

/* 10 different messages types, if more types are need please add and update MAX_NUM_OF_MSG_TYPES definition */
typedef enum {
	MESS_TYPE_A = 0,
	MESS_TYPE_B,
	MESS_TYPE_C,
	MESS_TYPE_D,
	MESS_TYPE_E,
	MESS_TYPE_F,
	MESS_TYPE_G,
	MESS_TYPE_H,
	MESS_TYPE_I,
	MESS_TYPE_J,
	MESS_TYPE_K
} messageTypes;

/* message frame structure */
struct messageFrame {
	messageTypes messageType;
	char payload[MAX_PAYLOAD_LEN];
};

typedef enum { ERR_EMPTY, ERR_UNKNOWN, ERR_INVALID_TYPE, ERR_OK } err_codes;

err_codes task_manage_msg();
err_codes fifo_register_callback(void (*handler)(struct messageFrame *msg), messageTypes num);
err_codes message_push(struct messageFrame msg);

#endif