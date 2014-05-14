#include <Arduino.h>
//#include <Bounce.h>          //used for debouncing the FN buttons
//#include <LiquidCrystal.h>
//#include <PString.h>
#include <avr/pgmspace.h>
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected

setup();
loop();
checkConnection();
goToSleep();

const int RESETPIN = A1;
volatile uchar usbSofCount;
bool sleeping = false;
const int SOFDELAY = 500;                                               //max time to wait in between SOF updates
unsigned long lastSOF = 0;
unsigned long previousMillis2 = 0;

    



void setup() {
    digitalWrite(RESETPIN, HIGH);                                               //this needs to be set high before anything so the device doesn't get reset 
    
    IO io();                                                                    //create the instance and set up all inputs and outputs
    Options opt();                                                              //create the instance and set up default options
    TweetHandler twt();                                                         //new instance of TweetHandler, doesn't require anything
    LCDControl lcd(opt, twt);                                                   //new instance, set up LCD and play the boot animation
    Comms comms(opt, io, twt);                                                  //new instance, set up usb comms
  
    comms.handshake();                                                          //establish a connection with the host program
}

//========================================================

void loop()
{
    
    
    
    
    
//  usbPoll();
//  //sleepWait();
//  
//  //if (readyToSleep == 1) {
//    sleepDetect();
//    sleepState();
//  //}
//  
//  if (sleepStatus == 0) {
//
//    newTweetBlink();
//    checkRainbow();
//    checkFN1();
//    checkFN2();
//    checkpot(); 
//  
//    checkcomms();
//    checkmode(); 
//    
//    checkfrozen();   //see if the lcd is supposed to be frozen, and if it needs to unfreeze
//
//    scrolltext();   
//    if (firstTweet == 0) {
//      if (waitforbegin == 0) {
//        if (greaterThan16 == 1) {
//          printtext();   //actually print the text
//        }
//      } 
//    }  
//  }
}




//=======================================================================

void checkpot() {
  finalSpeed = analogRead(SPEEDPIN) / 2;   //get the value of the speed potentiometer, divided by 2
  //finalspeed = potvalue / 2;   //compute that value
}




//keep sleep stuff out here actually

void IO::checkConnection() {                                                    //check if the device's data connection was disconnected, always run this
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


