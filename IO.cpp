//handles basic device IO
#include "IO.h"

//IO inout;

IO::IO(Options& optin) {                                                         //constructor, sets up all the inputs and outputs  
    opt = optin;
    digitalWrite(RESETPIN, HIGH);                                               //this needs to be set high before anything else so the device doesn't reset
    pinMode(RESETPIN, OUTPUT);
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    pinMode(REDLITE, OUTPUT);
    pinMode(GREENLITE, OUTPUT);
    pinMode(BLUELITE, OUTPUT);
    dbFN1 = Bounce();                                                           //set up the function button debouncing
    dbFN2 = Bounce(); 
    dbFN1.attach(FN1PIN);
    dbFN2.attach(FN2PIN);
    
    CONLED = A4;                                                  //connection led pin
    FN1PIN = 4;
    FN2PIN = A3;
    SPEEDPIN = A0;                                               //pin the speed pot is
    RESETPIN = A1;                                               //never needed to be implemented, but we're stuck with it now
    LCDPOWPIN = 16;                                              //pin used to control power to the contrast pot, turns contrast on/off
    REDLITE = 9;
    GREENLITE = 5;
    BLUELITE = 6;
    previousMillis = 0;
    previousMillis5 = 0;
    previousMillis6 = 0;
    BLINKTIME = 500;                                              //time between connection led state changes
    blinkState = false;                                                //controls whether the connection led needs to change states
    blinkEnabled = false;
    
    currentColor = 0;                                                  //current color section that is being faded though
    rainLevel = 0;
    rain = 0;
    blinkCount = 0;
    blinking = false;
}

void IO::connectionLED(byte mode) {                                             //controls the connection LED, needs a mode byte
    switch(mode) {
        case 0:                                                                 //turn the LED off
            digitalWrite(CONLED, LOW);
            break;  
        case 1:                                                                 //turn the LED on
            digitalWrite(CONLED, HIGH);
            break;
        case 2:                                                                 //blink the LED (non-blocking, must be continuously called to blink)
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

byte IO::checkButtons() {                                                       //checks the debounced buttons for any changes, needs to be called continuously
    if(dbFN1.update()) {                                                        //if fn1's state changed
        if(!dbFN1.read()) {                                                     //if the button was just released
            return 1;
        }                              
    }
    else if(dbFN2.update()) {                                                   //if fn2's state changed
        if(!dbFN2.read()) {                                                     //if the button was just released
            return 2;
        }
    }
    else {
        return 0;                                                               //no button state changes
    }
}

int IO::checkPot() {                                                            //used to check the speed pot position, should be called continuously
    return analogRead(SPEEDPIN) / 2;
}

//==============================================================================

void IO::setBacklight(uint8_t r, uint8_t g, uint8_t b, byte brightness) {       //set the backlight to a specific color and brightness
    r = map(r, 0, 255, 0, brightness);
    g = map(g, 0, 255, 0, brightness);
    b = map(b, 0, 255, 0, brightness);

    //common anode so invert
    r = map(r, 0, 255, 255, 0);
    g = map(g, 0, 255, 255, 0);
    b = map(b, 0, 255, 255, 0);
    
    red = r;
    green = g;
    blue = b;

    analogWrite(REDLITE, red);
    analogWrite(GREENLITE, green);
    analogWrite(BLUELITE, blue);
}

void IO::tweetBlink() {                                                         //used to trigger a tweet blink, should be called while waiting in the tweet beginning
    if (opt.getBlink()) {                                                       //check if tweetblink is enabled
        if(opt.getReadyBlink()) {                                               //check if we are currently blinking
            unsigned long currentMillis = millis();
            if(currentMillis - previousMillis5 > opt.getBlinkSpd()) {           //if it's time to change the backlight color
                previousMillis5 = currentMillis;
                if(blinkCount == 4) {                                           //done blinking
                    blinkCount = 0;                                             //reset blink count
                    opt.setReadyBlink(false);
                    //blinking = false;                                           //not blinking anymore
                }
                else if(blinkCount % 2 == 0) {                                  //on even numbered blinkcounts, set the backlight to the normal color
                    opt.updateCol();
                    blinkCount++;
                }
                else {                                                          //on odd numbered blinkcounts, set the backlight to the blink color
                    opt.updateBlinkCol();
                    blinkCount++;
                } 
            }
        }
    }
}

void IO::rainbow() {                                                            //controls the backlight's rainbow mode, must be ran continuously
    if(opt.getRainbow()) {
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis6 > rainSpeed) {                           //if it's to advance colors
            previousMillis6 = currentMillis;
            if(rainLevel < 255) {
                switch(currentColor) {                                              //fade through each different color
                    case 0:
                        opt.setCol(rain, 0, 255-rain);
                        break;
                    case 1:
                        opt.setCol(255-rain, rain, 0);
                        break;
                    case 2:
                        opt.setCol(0, 255-rain, rain);
                        break;
                }
                rainLevel++;
            }
            else {
                rainLevel = 0;
                currentColor++;
                if(currentColor == 3) {
                    currentColor = 0;
                }
            }
        }
    }
}