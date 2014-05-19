#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <Bounce.h>                                                             //required for debouncing
#include "Options.h"
#include "LCDControl.h"
#include "Comms.h"

#define CONLED A4                                                               //connection led pin
#define FN1PIN 4                                                                //FN1 button
#define FN2PIN A3                                                               //FN2 button
#define SPEEDPIN A0                                                             //pin the scroll speed pot is on
#define CONTRASTPIN 16                                                          //pin used to control power to the contrast pot, no longer needs to be used (just keep HIGH)
//#define RESETPIN A1                                                           //never needed to be implemented, but might as well keep it in here
//pins for each of the lcd's backlight leds
#define REDLITE 9
#define GREENLITE 5
#define BLUELITE 6

class IO {
    public:
        IO();
        void checkButtons();
        int checkPot();
        void connectionLED(byte mode);
        void setBacklight(uint8_t r, uint8_t g, uint8_t b, byte brightness);
        void tweetBlink();
        void rainbow();
    private:
        unsigned long previousMillis;
        unsigned long previousMillis5;
        unsigned long previousMillis6;
        int blinkTime;                                           
        bool blinkState;                                             
        bool blinkEnabled;
        byte blinkSpeed;                                                        
        byte currentColor;                                                  
        byte rainLevel;
        byte red;
        byte green;
        byte blue;
        byte bRed;
        byte bGreen;
        byte bBlue;
        Bounce dbFN1;
        Bounce dbFN2;
        byte blinkCount;               
};

#endif	/* IO_H */

