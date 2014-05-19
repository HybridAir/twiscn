//TwitterScreen Device code
//TODO: demo mode maybe, previous tweets
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
void doButtons();
void checkForSleep();
void prepare();

//global variables, shouldn't hurt anything
volatile uchar usbSofCount;                                                     //holds the current SOF count
bool sleeping = false;                                                          //stores the sleep status
const unsigned int SOFDELAY = 500;                                              //max time to wait in between SOF checks
unsigned long lastSOF = 0;                                                      //last time an SOF happened in ms
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
    doButtons();                                                                //monitors button changes and processes them
    lcd.setSpeed(inout.checkPot());                                             //applies any changes made to the speed pot
    inout.rainbow();                                                            //control the rainbow backlight changes                                                        
    lcd.scrollTweet();                                                          //scrolls the tweet
    inout.tweetBlink();                                                         //blinks the lcd if any new tweets are displayed
    checkForSleep();                                                            //checks if the device needs to be sleeping
}

//==============================================================================

void prepare() {                                                                //used to prepare the device for operation
    opt.defaults();                                                             //go back to all default options
    lcd.sleepLCD(false);                                                        //get the LCD going
    comms.handshake();                                                          //establish a connection with the host program
}

void doButtons() {                                                              //used to check the function buttons, must be continuously ran
    byte btn = inout.checkButtons();                                            //check for any button changes, get the button that was changed out
    if(btn != 0) {                                                              //if there was a change
        comms.sendBtn(btn);                                                     //send the button that changed to the host program
    }
}

//==============================================================================

void checkForSleep() {                                                          //checks if the device was disconnected, must be continuously ran
    unsigned long currentMillis = millis();                                     //get the current time
    unsigned long currentSOF = usbSofCount;                                     //get the current number of SOFs
    if(currentMillis - previousMillis2 > SOFDELAY) {                            //only check for new SOF counts every 500 ms (SOFDELAY)
        previousMillis2 = currentMillis;                                        //save the current time to use as a reference
        if(currentSOF == lastSOF) {                                             //if the SOF counts match (meaning we lost usb connection)  
            if(!sleeping) {                                                     //if we are not already sleeping
                sleeping = true;                                                //we will be now
                goToSleep();
            }
        }
        else {                                                                  //the SOF counts are different (they're being updated, so it's connected)
            lastSOF = currentSOF;                                               //save the current SOF to use as a reference
            sleeping = false;                                                   //set this to true to wake up if necessary
        }
    }
}

void goToSleep() {                                                              //used to bring the device down for "sleep"
    lcd.sleepLCD(true);                                                         //tell the lcd to sleep
    comms.setConnected(false);                                                  //tell comms that we are no longer connected to the host (so it will know to reconnect)
    inout.connectionLED(0);                                                     //turn the connection LED off, no longer connected
    while(sleeping) {                                                           //run some checks while the device is "sleeping"
        usbPoll();                                                              //keep checking the usb
        checkForSleep();                                                        //check if we still need to be sleeping
        if (!sleeping) {                                                        //if the previous method said it's time to wake up                                                                    
            prepare();                                                          //prepare the device for operation again, sort of like resetting
        }
    }
}