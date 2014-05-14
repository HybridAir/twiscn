#include <Arduino.h>
#include <HIDSerial.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"
#include "IO.h"                                                             //the usbSofCount variable requires this

Comms::Comms(Options optin, IO ioin) {                                                                //default constructor
    opt = optin;
    io = ioin;
}

void Comms::readComms() {                                                       //checks if we got anything new from the host, and then processes it
    if (usb.available()) {                                                      //check if there's something in the usb buffer
        usb.read((uint8_t*)usbBuffer);                                          //put the data into a string (probably dont need the uint8_t*)
        char inByte = usbBuffer.charAt(0);                                      //first character is used to identify the data packet type
    
        switch (inByte) {                                                       //check what character it is, and process accordingly
            case '=':                                                           //marks the end of the entire transfer, must always be in its own packet     
                checkType();                                                    //process the completed data transfer
                break;     
            default:                                                            //this will only trigger if we got transfer type signifier (!, @, etc)            
                transferOut += usbBuffer;                                       //add the current packet to the output transfer String
                break;
        }
    }
}

//might need to eventually clear transferOut

void Comms::checkType() {                                                       //used to check the type of transfer
    char type = transferOut.charAt(0);                                          //get the first char out of the transfer output, it's the transfer type
    transferOut = transferOut.substring(1);                                     //remove the the first character, it's no longer needed in there
    switch(type) {                                             //check the first char of the transferOut String
        case '@':                                                               //username transfer, also signifies the start of a new tweet
            userOut = transferOut;                                              //put the transfer into a new String for the username
            transferOut = "";
            gotUser = true;                               
            break;
        case '!':                                                               //tweet transfer, ending for a new tweet     
            twtOut = transferOut;                                               //put the transfer into a new String for the tweet
            transferOut = "";
            gotTweet = true;
            break;
        case '$':                                                               //option transfer
            opt.extractOption(transferOut);
            transferOut = "";
            break;   
    }
    if (gotUser & gotTweet) {
        twt.setUser(twtOut);                                                    //these will go somewhere
        twt.setTweet(userOut);
        gotTweet = false;                                                       //already got the new tweet, so reset those vars
        gotUser = false;
    }   
}

void Comms::handshake() {                                                       //function used to establish a data connection with the host
    while (!connected) {                                                        //do this while we are not connected
        connectingAnimation();                                                  //display the connecting animation on the LCD
        io.connectionLED(2);                                                    //blink the connection led to further signify that the device is connecting
        usbPoll();                                                              //keep polling the USB port for any new data
        usb.println("`");                                                       //continuously send this to the host so it knows the we are waiting for a handshake
        if (usb.available()) {                                                  //check if we got any data from the host
            usb.read((uint8_t*)usbBuffer);                                      //read that data into usbBuffer
            if (usbBuffer.charAt(0) == '~') {                                   //a "~" is the host acknowledging that it got the "`" from before
                connected = true;                                               //and now we are connected
            }
        }  
    }
    io.connectionLED(1);
    connectedLCD();  
}