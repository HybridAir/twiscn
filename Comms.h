#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>
#include <HIDSerial.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this

class Comms {
    public:
        Comms(Options optin);
        readComms();
        handshake();
    private:
        checkType();
        Options opt;
        volatile uchar usbSofCount;
        HIDSerial usb;                                                          //creates a new HIDSerial instance, named usb
        String usbBuffer;
        bool newOutput = false;
        String transferOut;
        String userOut;
        String twtOut;
        bool gotUser = false;
        bool gotTweet = false;
        bool connected = false;
        const int CONLED = A4;
};

#endif	/* COMMS_H */

