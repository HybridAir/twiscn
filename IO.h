#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this

class Comms {
    public:
        IO();
    private:
        volatile uchar usbSofCount;
};

#endif	/* IO_H */

