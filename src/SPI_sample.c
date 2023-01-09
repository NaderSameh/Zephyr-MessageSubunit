#include "SPI_sample.h"
#include "FifoHandler.h"

/* pushing demo data to the fifo */
void task_acc_push()
{
	char *str = "{\"ACC\": \"true\"}";
	struct messageFrame accerlometer;
	accerlometer.messageType = MESS_TYPE_B;
	strcpy(accerlometer.payload, str);
	message_push(accerlometer);
}
