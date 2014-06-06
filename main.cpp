//TwitterScreen Device code
//TODO: add handshake check while running
#include <Arduino.h>                                                            //used for its nice methods and stuff
#include "usbdrv.h"                                                             //needed for SOF counts
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include <LiquidCrystal.h>                                                      //used to control the LCD

//included class headers: 
#include "Comms.h"
#include "IO.h"
#include "LCDControl.h"
#include "Options.h"
#include "TweetHandler.h"

//prototype declaration

void setup();
void loop();
void checkConnection();
void goToSleep();
void checkAlive();
void prepare();

//global variables, shouldn't hurt anything
bool sleeping = false;                                                          //stores the sleep status
const unsigned int ALIVEDELAY = 1000;                                           //max time to wait in between keepAlive updates
unsigned long previousAlive = 0;                                                //last time an SOF happened in ms
unsigned long previousMillis2 = 0;                                              //used for keeping track of SOF checking times
const int LCDWIDTH = 16;                                                        //character width of the LCD
    
//global class initialization
IO inout;                                                                       //new instance of IO
LiquidCrystal lcdc(7, 8, 13, 10, 11, 12);                                       //new instance of the LiquidCrystal class, needs pins to use
Options opt;                                                                    //new instance of options
TweetHandler twt(LCDWIDTH);                                                     //new instance of TweetHandler, needs the LCDWIDTH
LCDControl lcd(LCDWIDTH);                                                       //new instance of LCDControl, needs the LCDWIDTH
Comms comms;                                                                    //new instance of comms

//==============================================================================

void setup() {  
    prepare();                                                                  //prepare the device for operation
}

void loop() {
    comms.readComms();                                                          //checks for any new comms data and processes it
    inout.checkButtons();                                                       //monitors button changes and processes them
    lcd.setSpeed(inout.checkPot());                                             //applies any changes made to the speed pot
    inout.rainbow();                                                            //control the rainbow backlight changes                                                        
    lcd.scrollTweet();                                                          //scrolls the tweet
    inout.tweetBlink();                                                         //blinks the lcd if any new tweets are displayed
    checkAlive();                                                               //checks if the device needs to be sleeping
}

void prepare() {                                                                //used to prepare the device for operation
    opt.defaults();                                                             //go back to all default options
    lcd.sleepLCD(false);                                                        //get the LCD going
    comms.handshake();                                                          //establish a connection with the host program
    previousMillis2 = millis();                                                 //set previousMillis2 to the current time in preparation for the first checkForSleep
}

//==============================================================================

void checkAlive() {                                                             //checks if the host died
    unsigned long currentMillis = millis();                                     //get the current time
     unsigned long currentAlive = comms.keepAlive;                              //get the current keepAlive value
     if(currentMillis - previousMillis2 > ALIVEDELAY) {                         //only check for new keepalives every ALIVEDELAY
         previousMillis2 = currentMillis;                                       //save the current time to use as a reference
         if(currentAlive == previousAlive) {                                    //if the keepAlive values match (meaning we lost program/host connection)  
             if(!sleeping) {                                                    //if we are not already sleeping
                 sleeping = true;                                               //we will be now
                 goToSleep();
             }
         }
         else {                                                                 //the SOF counts are different (they're being updated, so it's connected)
             previousAlive = currentAlive;                                      //save the current SOF to use as a reference
             sleeping = false;                                                  //set this to true to wake up if nec
         }
     }
}

void goToSleep() {                                                              //used to make the device sleep, usually after the host died
    comms.setConnected(false);                                                  //tell comms that we are no longer connected to the host (so it can reconnect when we wake up)
    lcd.sleepLCD(true);                                                         //tell the lcd to sleep
    inout.connectionLED(0);                                                     //turn the connection LED off, no longer connected
    while(sleeping) {                                                           //run some checks while the device is "sleeping"
        comms.readComms();                                                      //checks for any new comms data, there could be a new keepAlive packet
        if(comms.keepAlive != previousAlive) {                                  //check if we still need to be sleeping
            sleeping = false;                                                   //time to wake up
            prepare();
        }
    }
}