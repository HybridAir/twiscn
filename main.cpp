#include <Arduino.h>
#include "usbdrv.h"
#include "Options.h"
#include "IO.h"                                                             //needed for SOF counts
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected

//maybe add a demo mode?

void setup();
void loop();
void checkConnection();
void goToSleep();
void doButtons();
void doSpeedPot();
void checkForSleep();
void prepare();
void doRainbow();
void doScrolling();

volatile uchar usbSofCount;                                                     //holds the current SOF count
bool sleeping = false;
const int SOFDELAY = 500;                                                       //max time to wait in between SOF checks
unsigned long lastSOF = 0;
unsigned long previousMillis2 = 0;
const int LCDWIDTH = 16;                                                //character width of the LCD
    
IO inout;
Options opt;
TweetHandler twt;
LCDControl lcd;
Comms comms;

//==============================================================================

void setup() {  
    inout = IO inout();                                                         //create the instance and set up all inputs and outputs
    opt = Options opt();                                                        //create the instance and set up default options
//    lcd = LCDControl lcd(opt);                                                  //new instance, set up LCD and play the boot animation
//    twt = TweetHandler twt(lcd);                                                //create the instance for use by other classes
    twt = TweetHandler twt(LCDWIDTH);                                                //create the instance for use by other classes
    lcd = LCDControl lcd(opt, twt, LCDWIDTH);                                                  //new instance, set up LCD and play the boot animation
    comms = Comms comms(opt, inout, twt, lcd);                                       //new instance, set up usb comms
    
    prepare();                                                                  //prepare the device for operation
}

void loop() {
    comms.readComms();
    doButtons();
    doSpeedPot();
    doRainbow();
    doScrolling();
    checkForSleep();
}

//==============================================================================

void prepare() {                                                                //used to prepare the device for operation
    lcd.sleepLCD(false);                                                        //get the LCD going
    comms.handshake();                                                          //establish a connection with the host program
}

void doButtons() {                                                              //used to check the function buttons, must be continuously ran
    byte btn = inout.checkButtons();
    if(btn != 0) {
        comms.sendBtn(btn);
    }
}

void doSpeedPot() {                                                             //used to check the speed pot, must be continuously ran
    lcd.setSpeed(inout.checkPot());                                             //send the lcd the current pot position that inout got
}

void doRainbow() {                                                              //used to update the rainbow mode backlight color, must be continuously ran
    if(opt.getRainbow()) {
        inout.rainbow();
    }
}

void doScrolling() {
    lcd.scrollTweet();
}

//==============================================================================

void checkForSleep() {                                                          //checks if the device was disconnected, must be continuously ran
    unsigned long currentMillis = millis();                                     //get the current time
    unsigned long currentSOF = usbSofCount;                                     //get the current number of SOFs
    if(currentMillis - previousMillis2 > SOFDELAY) {                            //only check for new SOF counts every 500 ms (SOFDELAY)
        previousMillis2 = currentMillis;                                        //save the current time to use as a reference
        if(currentSOF == lastSOF) {                                             //if the SOF counts match (meaning we lost usb connection)  
            if(!sleeping) {                                                     //if we are not already sleeping
                //need this if here since this function will be called while the device is asleep
                //don't want it going into some crazy sleep limbo
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
    inout.connectionLED(0);                                                     //turn the connection LED off, no longer connected
    while(sleeping) {                                                           //run some checks while the device is "sleeping"
        usbPoll();                                                              //keep checking the usb
        checkForSleep();                                                        //check if we still need to be sleeping
        if (!sleeping) {                                                        //if the previous method said it's time to wake up                                                     
            lcd.sleepLCD(false);                                                //wake the LCD up                 
            prepare();                                                          //prepare the device for operation again, sort of like resetting
            //pins are still set, and options are still set
        }
    }
}