#include "FifoHandler.h"

/* 
* Fifo defintion, it is a ring buffer that has start and end pointers to keep track of the logged messages 
* in case the fifo is full, the oldest record is overwriten.
* the number of messages the fifo can store at the same time is defined by FIFO_MAX_SIZE definition
*/
static struct message_handler_def {
	int startPointer;
	int endPointer;
	struct messageFrame myMessageFrame[FIFO_MAX_SIZE];
} myMessageHandler;

/* array of function pointers that acts as callback handlers for different messages types */
static void (*messages_handler[MAX_NUM_OF_MSG_TYPES])(struct messageFrame *msg);

/* should be used from different threads to push messages from different subunits in the system */
err_codes message_push(struct messageFrame msg)
{
	err_codes err = ERR_OK;
	/* storing the incoming in the fifo */
	myMessageHandler.myMessageFrame[myMessageHandler.endPointer].messageType = msg.messageType;
	strcpy(myMessageHandler.myMessageFrame[myMessageHandler.endPointer].payload, msg.payload);
	myMessageHandler.endPointer++;
	/* using the ring buffer technique when endpointer exceed delimiter */
	if (myMessageHandler.endPointer >= FIFO_MAX_SIZE)
		myMessageHandler.endPointer = 0;
	/* erase the new message frame for the next message */
	memset((void *)&(myMessageHandler.myMessageFrame[myMessageHandler.endPointer]), 0,
	       sizeof(myMessageHandler.myMessageFrame[0]));
	/* fifo full? drop the oldest message */
	if (myMessageHandler.endPointer == myMessageHandler.startPointer) {
		myMessageHandler.startPointer++;
		/* loop if exceed delimiter also */
		if (myMessageHandler.startPointer >= FIFO_MAX_SIZE)
			myMessageHandler.startPointer = 0;
	}
	/* return error if fifo is empty after a push */
	if (myMessageHandler.endPointer == myMessageHandler.startPointer)
		err = ERR_UNKNOWN;

	return err;
}

/* 
* fetch the oldest message from fifo to the pointer in arrgument
* param is fifo message frame structure
* return ERR_OK upon success
*/
static err_codes message_pull(struct messageFrame *msg)
{
	err_codes err;
	err = ERR_OK;
	/* return empty if fifo is empty */
	if (myMessageHandler.endPointer == myMessageHandler.startPointer) {
		err = ERR_EMPTY;
		return err;
	}
	/* pull the oldest message frame from fifo to the frame pointer */
	*msg = myMessageHandler.myMessageFrame[myMessageHandler.startPointer];
	/* check frame sanity */
	if (strlen(msg->payload) == 0)
		err = ERR_EMPTY;
	if (msg->messageType > MAX_NUM_OF_MSG_TYPES || msg->messageType < 0)
		err = ERR_INVALID_TYPE;
	return err;
}

/* Remove the oldest message from fifo
*  return ERR_OK upon success
*/
static err_codes message_consume()
{
	err_codes err = ERR_OK;
	myMessageHandler.startPointer++;
	/* return empty error if fifo is empty */
	if (myMessageHandler.startPointer == myMessageHandler.endPointer) {
		err = ERR_EMPTY;
	}
	/* reach delimiter? loop back */
	if (myMessageHandler.startPointer >= FIFO_MAX_SIZE) {
		myMessageHandler.startPointer = 0;
	}
	return err;
}
/* 
* Pull messages from fifo and call corresponding message handler/consumer.
* it dispatches the message upon calling the handler
* return ERR_OK upon success, ERR_EMPTY if fifo empty & ERR
*/
err_codes task_manage_msg()
{
	struct messageFrame msg;
	/* pull the message frame */
	err_codes err = message_pull(&msg);
	if (err != ERR_OK)
		return err;
	/* call corresponding handler */
	messages_handler[msg.messageType](&msg);
	/* dispatch message frame */
	err = message_consume();
	return err;
}

/*
* assigning callback handlers for different message types
* return ERR_OK upon success
*/
err_codes fifo_register_callback(void (*handler)(struct messageFrame *msg), messageTypes num)
{
	err_codes err;
	err = ERR_OK;
	/* return error if pointer is NULL */
	if (handler == NULL)
		err = ERR_INVALID_TYPE;
	/* assign pointer to function to the matching data type index */
	messages_handler[num] = handler;
	return err;
}