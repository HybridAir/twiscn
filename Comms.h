#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>
#include <HIDSerial.h>
#include "Options.h"
#include "IO.h"
#include "TweetHandler.h"
#include "LCDControl.h"
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"          //the usbSofCount variable requires this

class Comms {
    public:
        Comms();
        void readComms();
        void handshake();
        void sendBtn(byte in);
    private:
        void checkType();
        //Options opt;
        //IO io;
        //TweetHandler twt;
        HIDSerial usb;                                                          //creates a new HIDSerial instance, named usb
        //LCDControl lcd;
        char usbBuffer[32];
        String usbBufStr;
        //String usbBuffer;
        String transferOut;
        String userOut;
        String twtOut;
        bool gotUser;
        bool gotTweet;
        bool connected;
};

#endif	/* COMMS_H */

