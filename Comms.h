#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>
#include <HIDSerial.h>
#include "Options.h"
#include "IO.h"
#include "TweetHandler.h"
#include "LCDControl.h"
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this (and other stuff too I think)  

class Comms {
    public:
        Comms();
        void readComms();
        void handshake();
        void sendBtn(char in);
        void setConnected(bool in);
        void connect();
        unsigned long keepAlive;
    private:
        void checkType();
        HIDSerial usb;                                                          //creates a new HIDSerial instance, named usb
        char usbBuffer[32];
        String usbBufStr;
        String transferOut;
        String userOut;
        String twtOut;
        String versions;
        bool gotUser;
        bool gotTweet;
        bool connected;
};

#endif	/* COMMS_H */

