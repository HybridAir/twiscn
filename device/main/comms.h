#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>

#include "usbdrv.h"



#include <TwiscnUSB.h>


void commsInit();
void get_input();
void monitorConnection();


#endif