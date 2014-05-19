//handles device IO control
#include "IO.h"

extern Options opt;                                                             //needed for Options class access
extern LCDControl lcd;                                                          //needed for LCDControl class access

IO::IO() {                                                                      //default constructor 
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    pinMode(REDLITE, OUTPUT);
    pinMode(GREENLITE, OUTPUT);
    pinMode(BLUELITE, OUTPUT);   
    //following is needed to prevent backlight from flashing on when device is powered
    digitalWrite(REDLITE, HIGH);
    digitalWrite(GREENLITE, HIGH);
    digitalWrite(BLUELITE, HIGH);
    //set up the function button debouncing
    dbFN1 = Bounce();                                                           
    dbFN2 = Bounce(); 
    dbFN1.attach(FN1PIN);
    dbFN2.attach(FN2PIN);
    //set necessary variable values
    previousMillis = 0;                                                         //used within connectionLED for non-blocking delay
    previousMillis5 = 0;                                                        //used within tweetBlink for non-blocking delay
    previousMillis6 = 0;                                                        //used within rainbow for non-blocking delay
    blinkTime = 500;                                                            //time between connection animation state changes
    blinkState = false;                                                         //controls whether the connection led needs to change states
    blinkEnabled = false;                                                       
    currentColor = 0;                                                           //current color section of the rainbow that is being faded though
    rainLevel = 0;                                                              //current brightness level of the backlight leds being faded
    blinkCount = 0;                                                             //amount of times the backlight changed colors during a tweet blink                                                          
    
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
            if(currentMillis - previousMillis > blinkTime) {                    //if it is time to advance the blinkState
                previousMillis = currentMillis;
                lcd.connectAnim();                                              //also advance the connecting animation on the lcd
                if (blinkState) {
                    blinkState = 0;
                }
                else {
                    blinkState = 1;
                }
                connectionLED(blinkState);                                      //update the connection led with the new power value
            }
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
            if(currentMillis - previousMillis5 > opt.getBlinkSpd()) {           //if it's time to change the backlight color (blink)
                previousMillis5 = currentMillis;
                if(blinkCount == 5) {                                           //done blinking
                    blinkCount = 0;                                             //reset blink count
                    opt.setReadyBlink(false);                                   //no longer blinking
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
        if(currentMillis - previousMillis6 > opt.getRainSpd()) {                //if it's time to advance colors
            previousMillis6 = currentMillis;
            if(rainLevel <= 255) {                                              //fade through each color section 255 times
                switch(currentColor) {                                          //fade through each different color section
                    case 0:
                        opt.setCol(rainLevel, 0, 255-rainLevel);
                        break;
                    case 1:
                        opt.setCol(255-rainLevel, rainLevel, 0);
                        break;
                    case 2:
                        opt.setCol(0, 255-rainLevel, rainLevel);
                        break;
                }
                rainLevel++;
            }
            else {
                rainLevel = 0;                                                  //reset the rainlevel
                currentColor++;                                                 //move to the next color section
                if(currentColor == 3) {                                         //go to the beginning if needed
                    currentColor = 0;
                }
            }
        }
    }
}