#include <Arduino.h>
#include "usbdrv.h"
#include "IO.h"
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected

void setup();
void loop();
void checkConnection();
void goToSleep();
void doButtons();
void doSpeedPot();
void doSleep();

volatile uchar usbSofCount;
bool sleeping = false;
const int SOFDELAY = 500;                                               //max time to wait in between SOF updates
unsigned long lastSOF = 0;
unsigned long previousMillis2 = 0;
    
IO inout;
Options opt;
TweetHandler twt;
LCDControl lcd;
Comms comms;

//==============================================================================

void setup() {  
    inout = IO inout();                                                         //create the instance and set up all inputs and outputs
    opt = Options opt();                                                        //create the instance and set up default options
    twt = TweetHandler twt();                                                   //new instance of TweetHandler, doesn't require anything
    lcd = LCDControl lcd(opt, twt);                                             //new instance, set up LCD and play the boot animation
    comms = Comms comms(opt, inout, twt);                                       //new instance, set up usb comms
    
    comms.handshake();                                                          //establish a connection with the host program
}

void loop() {
    comms.readComms();
    doButtons();
    doSpeedPot();
    doSleep();
}

//==============================================================================

void doButtons() {
    byte btn = inout.checkButtons();
    if(btn != 0) {
        comms.sendBtn(btn);
    }
}

void doSpeedPot() {
    lcd.setSpeed(inout.checkPot());
}

void checkConnection() {                                                    //check if the device's data connection was disconnected, always run this
    unsigned long currentMillis = millis();
    unsigned long currentSOF = usbSofCount;                                     //get the current number of SOFs
    if(currentMillis - previousMillis2 > SOFDELAY) {                            //only check for new SOF counts every 500 ms (SOFDELAY)
        previousMillis2 = currentMillis;  
        if(currentSOF == lastSOF) {                                             //if the SOF counts match (meaning we lost usb connection)  
            if(!sleeping) {
                sleeping = true;
                goToSleep();
            }
        }
        else {                                                                  //the SOF counts are different (still being updated, so it's still connected)
            lastSOF = currentSOF;
            sleeping = false;
        }
    }
}

void IO::goToSleep() {
    lcd.sleepLCD(true);                                                         //turn the LCD off (sleep))
    connectionLED(0);                                                           //turn the connection LED off, no longer connected
    while(sleeping) {                                                           //run some checks while the device is "sleeping"
        usbPoll();
        sleepDetect();                                                          //check if we still need to be sleeping
        if (!sleeping) {                                                        //if the previous method said it's time to wake up                                                     
            lcd.sleepLCD(false);                                                //wake the LCD up                 
            hostHandshake();                                                    //reconnect to the host
        }
    }
}