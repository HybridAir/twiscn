#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <Bounce.h>
#include "Options.h"



class IO {
    public:
        IO();
        void connectionLED(byte mode);
        byte checkButtons();
        int checkPot();
        void setBacklight(uint8_t r, uint8_t g, uint8_t b, byte brightness);
        //enableTweetBlink(bool in);
        void tweetBlink();
        //void setBlinkSpeed(byte in);
        void rainbow();
    private:
        //Options opt;
         int CONLED;                                                  //connection led pin
         char FN1PIN;
         char FN2PIN;
         char SPEEDPIN;                                               //pin the speed pot is
         char RESETPIN;                                               //never needed to be implemented, but we're stuck with it now
         char LCDPOWPIN;                                              //pin used to control power to the contrast pot, turns contrast on/off
         char REDLITE;
         char GREENLITE;
         char BLUELITE;
        unsigned long previousMillis;
        unsigned long previousMillis5;
        unsigned long previousMillis6;
         int BLINKTIME;                                              //time between connection led state changes
        bool blinkState;                                                //controls whether the connection led needs to change states
        bool blinkEnabled;
        byte blinkSpeed;                                                        //time between tweet blinks
        int rainSpeed;                                                          //time between rainbow increments
        byte currentColor;                                                  //current color section that is being faded though
        byte rainLevel;
        byte red;
        byte green;
        byte blue;
        byte bRed;
        byte bGreen;
        byte bBlue;
        byte rain;
        Bounce dbFN1;
        Bounce dbFN2;
        byte blinkCount;
                bool blinking;
};

//extern IO inout;

#endif	/* IO_H */

