#ifndef COMMS_H
#define	COMMS_H

#include <Arduino.h>
#include <HIDSerial.h>

class Comms {
    public:
        Comms(Options optin, IO ioin);
        readComms();
        handshake();
        sendBtn(byte in);
    private:
        checkType();
        Options opt;
        IO io;
        TweetHandler twt;
        HIDSerial usb;                                                          //creates a new HIDSerial instance, named usb
        String usbBuffer;
        bool newOutput = false;
        String transferOut;
        String userOut;
        String twtOut;
        bool gotUser = false;
        bool gotTweet = false;
        bool connected = false;
};

#endif	/* COMMS_H */

