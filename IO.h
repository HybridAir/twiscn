#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <Bounce.h>

class IO {
    public:
        IO();
        connectionLED(int in);
        checkButtons();
        checkConnection();
    private:
        goToSleep();
        volatile uchar usbSofCount;
        const int CONLED = A4;
        const char FN1PIN = 4;
        const char FN2PIN = A3;
        const char SPEEDPIN = A0;
        const char RESETPIN = A1;
        const char LCDPOWPIN = 16;      //pin used to control the contrast pot, and turn contrast on or off
        unsigned long previousMillis = 0;
        unsigned long previousMillis2 = 0;
        unsigned long lastSOF = 0;
        const int BLINKTIME = 500;
        const int SOFDELAY = 500;                                               //max time to wait in between SOF updates
        bool blinkState = false;
        Bounce dbFN1;
        Bounce dbFN2;
        Comms comms;
        LCDControl lcd;
        bool sleeping = false;
};

#endif	/* IO_H */

