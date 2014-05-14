//handles basic device IO
#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <Bounce.h>

IO::IO(LCDControl lcdin) {                                       //default constructor, sets up all the inputs and outputs                                                
    pinMode(RESETPIN, OUTPUT);
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    dbFN1 = Bounce(); 
    dbFN2 = Bounce(); 
    dbFN1.attach(FN1PIN);
    dbFN2.attach(FN2PIN);
    //comms = commsin;
    lcd = lcdin;
}

void IO::connectionLED(byte mode) {                                             //controls the connection LED, needs a mode
    switch(mode) {
        case 0:                                                                 //turn the LED off
            digitalWrite(CONLED, LOW);
            break;  
        case 1:                                                                 //turn the LED on
            digitalWrite(CONLED, HIGH);
            break;
        case 2:                                                                 //blink the LED (non-blocking)
            unsigned long currentMillis = millis();
            if(currentMillis - previousMillis > BLINKTIME) {
                previousMillis = currentMillis;
                if (blinkState) {
                    blinkState = 0;
                }
                else {
                    blinkState = 1;
                }
                connectionLED(blinkState);
            }
            break;
    }
}

//make that return a 1 or 2 instead, send it to comms in main
void IO::checkButtons() {                                                       //checks the debounced buttons for any changes, always run this
    if(dbFN1.update()) {
        if(!dbFN1.read()) {                                                     //if the button was just released
            comms.sendBtn(1);                                                   //tell the host the user pushed FN1
        }                              
    }
    else if(dbFN2.update()) {
        if(!dbFN2.read()) {                                                     //if the button was just released
            comms.sendBtn(2);                                                   //tell the host the user pushed FN1
        }
    }
}

//==============================================================================

//void IO::checkConnection() {                                                    //check if the device's data connection was disconnected, always run this
//    unsigned long currentMillis = millis();
//    unsigned long currentSOF = usbSofCount;                                     //get the current number of SOFs
//    if(currentMillis - previousMillis2 > SOFDELAY) {                            //only check for new SOF counts every 500 ms (SOFDELAY)
//        previousMillis2 = currentMillis;  
//        if(currentSOF == lastSOF) {                                             //if the SOF counts match (meaning we lost usb connection)  
//            if(!sleeping) {
//                sleeping = true;
//                goToSleep();
//            }
//        }
//        else {                                                                  //the SOF counts are different (still being updated, so it's still connected)
//            lastSOF = currentSOF;
//            sleeping = false;
//        }
//    }
//}
//
//void IO::goToSleep() {
//    lcd.sleepLCD(true);                                                         //turn the LCD off (sleep))
//    connectionLED(0);                                                           //turn the connection LED off, no longer connected
//    while(sleeping) {                                                           //run some checks while the device is "sleeping"
//        usbPoll();
//        sleepDetect();                                                          //check if we still need to be sleeping
//        if (!sleeping) {                                                        //if the previous method said it's time to wake up                                                     
//            lcd.sleepLCD(false);                                                //wake the LCD up                 
//            hostHandshake();                                                    //reconnect to the host
//        }
//    }
//}