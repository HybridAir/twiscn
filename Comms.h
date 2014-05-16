#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>
#include <HIDSerial.h>
#include "Options.h"
#include "IO.h"
#include "TweetHandler.h"
#include "LCDControl.h"

class Comms {
    public:
        Comms();
        void readComms();
        void handshake();
        void sendBtn(byte in);
    private:
        void checkType();
        Options opt;
        IO io;
        TweetHandler twt;
        HIDSerial usb;                                                          //creates a new HIDSerial instance, named usb
        LCDControl lcd;
        String usbBuffer;
        String transferOut;
        String userOut;
        String twtOut;
        bool gotUser;
        bool gotTweet;
        bool connected;
};

#endif	/* COMMS_H */

