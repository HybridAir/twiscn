//handles basic device IO
#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <Bounce.h>

IO::IO() {                                       //default constructor, sets up all the inputs and outputs                                                
    pinMode(RESETPIN, OUTPUT);
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    dbFN1 = Bounce(); 
    dbFN2 = Bounce(); 
    dbFN1.attach(FN1PIN);
    dbFN2.attach(FN2PIN);
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
        default:
            break;
    }
}

byte IO::checkButtons() {                                                       //checks the debounced buttons for any changes, always run this
    if(dbFN1.update()) {
        if(!dbFN1.read()) {                                                     //if the button was just released
            return 1;
            //comms.sendBtn(1);                                                   //tell the host the user pushed FN1
        }                              
    }
    else if(dbFN2.update()) {
        if(!dbFN2.read()) {                                                     //if the button was just released
            return 2;
            //comms.sendBtn(2);                                                   //tell the host the user pushed FN1
        }
    }
}