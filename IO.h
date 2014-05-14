#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include <Bounce.h>

class IO {
    public:
        IO();
        connectionLED(byte mode);
        checkButtons();
    private:
        const int CONLED = A4;
        const char FN1PIN = 4;
        const char FN2PIN = A3;
        const char SPEEDPIN = A0;
        const char RESETPIN = A1;
        const char LCDPOWPIN = 16;      //pin used to control the contrast pot, and turn contrast on or off
        unsigned long previousMillis = 0;
        const int BLINKTIME = 500;
        bool blinkState = false;
        Bounce dbFN1;
        Bounce dbFN2;
};

#endif	/* IO_H */

