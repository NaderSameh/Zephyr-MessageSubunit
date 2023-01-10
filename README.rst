
Message/Fifo Subunit Example
####################

Overview
********

This example demonstrates pushing payload messages from different threads
and handling those messages with specific handlers.

handlers are assigned to specific message types in application layer 

Demo threads are initialized to push sample data from sensors/modems to be proccessed 
by the fifo subunit


Requirements
************

The software is built upon Zephyr RTOS, it is built and ran on nRF52833 with the below output

Running output
**************
..
TEMP READING = {"T": "25"}

ACC READING = {"ACC": "true"}

ACC READING = {"ACC": "true"}

TEMP READING = {"T": "25"}

ACC READING = {"ACC": "true"}

ACC READING = {"ACC": "true"}

MODEM RESPONSE = {"GPS": "30.3123213,31.231231"}

....
