//used to communicate with the host computer/program over USB
#include <Arduino.h>
#include <HIDSerial.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected

Comms::Comms(Options optin, IO ioin, TweetHandler twtin, LCDControl lcdin) {    //onstructor
    opt = optin;
    io = ioin;
    twt = twtin;
    lcd = lcdin;
    usb.begin();                                                                //start the hidserial connection
}

void Comms::readComms() {                                                       //checks if we got anything new from the host, and then processes it
    usbPoll();
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

void Comms::checkType() {                                                       //used to check the type of transfer
    char type = transferOut.charAt(0);                                          //get the first char out of the transfer output, it's the transfer type
    transferOut = transferOut.substring(1);                                     //remove the the first character, it's no longer needed in there
    switch(type) {                                                              //check the first char of the transferOut String
        case '@':                                                               //username transfer, also signifies the start of a new tweet
            userOut = transferOut;                                              //put the transfer into a new String for the username
            transferOut = "";
            gotUser = true;                               
            break;
        case '!':                                                               //tweet transfer, ending for a new tweet     
            twtOut = transferOut;                                               //put the transfer into a new String for the tweet
            transferOut = "";                                                   //empty the transferOut to make room for a new one
            gotTweet = true;
            break;
        case '$':                                                               //option transfer
            opt.extractOption(transferOut);                                     //get the option data out of the transfer
            transferOut = "";                                                   //empty the transferOut to make room for a new one
            break;   
    }
    if (gotUser & gotTweet) {                                                   //if we got both the tweet and the user
        twt.setUser(twtOut);                                                    //give the tweet handler a new user
        twt.setTweet(userOut);                                                  //give the tweet handler a new tweet
        lcd.printNewTweet();                                                    //tell LCDControl to print the new tweet
        gotTweet = false;                                                       //already got the new tweet, so reset those vars
        gotUser = false;
    }   
}

void Comms::handshake() {                                                       //used to establish a data connection with the host
    while (!connected) {                                                        //do this while we are not connected
        lcd.connectAnim(true);                                                  //display the connecting animation on the LCD
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
    io.connectionLED(1);                                                        //turn the connection led solid on since we're connected now
    lcd.connectAnim(false);                                                     //show the connected notice on the lcd
}

void Comms::sendBtn(byte in) {                                                  //used to send button presses to the host program for processing
    usb.println(in);
    usb.println("=");
}