//handles basic device IO
#include <Arduino.h>
#include <Bounce.h>

IO::IO() {                                                                      //default constructor, sets up all the inputs and outputs                                                
    digitalWrite(RESETPIN, HIGH);                                               //this needs to be set high before anything else so the device doesn't reset
    pinMode(RESETPIN, OUTPUT);
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    dbFN1 = Bounce();                                                           //set up the function button debouncing
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
        }                              
    }
    else if(dbFN2.update()) {
        if(!dbFN2.read()) {                                                     //if the button was just released
            return 2;
        }
    }
    else {
        return 0;
    }
}

int IO::checkPot() {
    return analogRead(SPEEDPIN) / 2;
}