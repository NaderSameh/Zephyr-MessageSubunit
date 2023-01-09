#include "I2C_sample.h"
#include "FifoHandler.h"

/* pushing demo data to the fifo */
void task_temp_push()
{
	/* demo test temperature data */
	char *str = "{\"T\": \"25\"}";
	struct messageFrame temperature;
	temperature.messageType = MESS_TYPE_A;
	strcpy(temperature.payload, str);
	message_push(temperature);
}