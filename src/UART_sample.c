#include "UART_sample.h"
#include "FifoHandler.h"

/* pushing demo data to the fifo */
void task_uart_push()
{
	char *str = "{\"GPS\": \"30.3123213,31.231231\"}";
	struct messageFrame GPS;
	GPS.messageType = MESS_TYPE_C;
	strcpy(GPS.payload, str);
	message_push(GPS);
}
