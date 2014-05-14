#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <Bounce.h>

class IO {
    public:
        IO();
    private:
        volatile uchar usbSofCount;
};

#endif	/* IO_H */

