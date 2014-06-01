//used to communicate with the host computer/program over USB
#include "Comms.h"

extern Options opt;
extern IO inout;
extern TweetHandler twt;
extern LCDControl lcd;

Comms::Comms() {                                                                //default constructor
    usb.begin();                                                                //start up the usb hidserial connection
    gotUser = false;
    gotTweet = false;
    connected = false;                                                          //considering that this was just started, we will not be connected yet
    hardVersion = "$1v1a";                                                      //hardware version 1a
    firmVersion = "$2v1a";                                                      //firmware version 1a
}

void Comms::readComms() {                                                       //checks if we got anything new from the host, and then processes it, run this continuously
    usbPoll();                                                                  //make sure to run this as often as possible
    if (usb.available()) {                                                      //check if there's something in the usb buffer
        usb.read((uint8_t*)usbBuffer);                                          //put the data into a string
        char inByte = (uint8_t)usbBuffer[0];                                    //first character is used to identify the data packet type
        usbBufStr = String(usbBuffer);
    
        switch (inByte) {                                                       //check what character it is, and process accordingly
            case '=':                                                           //marks the end of the entire transfer, must always be in its own packet     
                checkType();                                                    //process the completed data transfer
                break;     
            default:                                                            //this will only trigger for regular packet transfers           
                transferOut += usbBufStr;                                       //add the current packet to the output transfer String
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
            transferOut = "";                                                   //empty transferOut so it can accept a new transfer
            gotUser = true;                                                     //we got the user               
            break;
        case '!':                                                               //tweet transfer, ending of a new tweet     
            twtOut = transferOut;                                               //put the transfer into a new String for the tweet
            transferOut = "";                                                   //empty transferOut so it can accept a new transfer
            gotTweet = true;                                                    //we got the tweet text
            break;
        case '$':                                                               //option transfer
            String out = transferOut;                                           //get the transferOut into a new string
            transferOut = "";                                                   //empty transferOut so it can accept a new transfer
            opt.extractOption(out);                                             //get the option data out of the transfer
            break;   
    }
    if (gotUser & gotTweet) {                                                   //if we got both the tweet and the user
        twt.setUser(userOut);                                                   //give the tweet handler a new user
        twt.setTweet(twtOut);                                                   //give the tweet handler a new tweet
        lcd.printNewTweet(true);                                                //tell LCDControl to print the new tweet
        //already got the new tweet, so reset those vars
        gotTweet = false;                                                       
        gotUser = false;
    }   
}

void Comms::handshake() {                                                       //used to establish a data connection with the host
    while (!connected) {                                                        //do this while we are not connected
        lcd.connectDisplay(true);                                               //display the connecting animation on the LCD
        inout.connectionLED(2);                                                 //blink the connection led to further signify that the device is connecting
        usbPoll();                                                              //keep polling the USB port for any new data
        usb.println("`");                                                       //continuously send this to the host so it knows the we are waiting for a handshake
        if (usb.available()) {                                                  //check if we got any data from the host
            usb.read((uint8_t*)usbBuffer);                                      //read that data into usbBuffer
            if ((uint8_t)usbBuffer[0] == '~') {                                 //a "~" is the host acknowledging that it got the "`" from before
                connected = true;                                               //and now we are connected
            }
        }  
    }
    delay(50);                                                                  //give the host a little time to get ready
    char ver[5];                                                                //get a char array ready
    hardVersion.toCharArray(ver, 6);                                                //put that String into that new char array
    usb.println(ver);                                                           //send the device version to the host
    delay(50);                                                                  //give the host a little time to get ready
    firmVersion.toCharArray(ver, 6);                                                //put that String into that new char array
    usb.println(ver);                                                           //send the device version to the host
    inout.connectionLED(1);                                                     //turn the connection led solid on since we're connected now
    lcd.connectDisplay(false);                                                  //show the connected notice on the lcd
}

void Comms::setConnected(bool in) {                                             //sets the connected status
    connected = in;
}

void Comms::sendBtn(char in) {                                                  //used to send button presses to the host program for processing
    usb.println(in);
    usb.println("=");
}