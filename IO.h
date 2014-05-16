#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <Bounce.h>

class IO {
    public:
        IO();
        connectionLED(byte mode);
        checkButtons();
        checkPot();
        setBacklight(uint8_t r, uint8_t g, uint8_t b, byte brightness);
        enableTweetBlink(bool in);
        tweetBlink();
        setBlinkSpeed(byte in);
        rainbow();
    private:
        const int CONLED = A4;                                                  //connection led pin
        const char FN1PIN = 4;
        const char FN2PIN = A3;
        const char SPEEDPIN = A0;                                               //pin the speed pot is
        const char RESETPIN = A1;                                               //never needed to be implemented, but we're stuck with it now
        const char LCDPOWPIN = 16;                                              //pin used to control power to the contrast pot, turns contrast on/off
        const char REDLITE = 9;
        const char GREENLITE = 5;
        const char BLUELITE = 6;
        unsigned long previousMillis = 0;
        unsigned long previousMillis5 = 0;
        unsigned long previousMillis6 = 0;
        const int BLINKTIME = 500;                                              //time between connection led state changes
        bool blinkState = false;                                                //controls whether the connection led needs to change states
        bool blinkEnabled = false;
        byte blinkSpeed;                                                        //time between tweet blinks
        int rainSpeed;                                                          //time between rainbow increments
        byte currentColor = 0;                                                  //current color section that is being faded though
        byte rainLevel = 0;
        byte red;
        byte green;
        byte blue;
        byte bRed;
        byte bGreen;
        byte bBlue;
        byte rain = 0;
        Bounce dbFN1;
        Bounce dbFN2;
        byte blinkCount = 0;
                bool blinking = false;
};

#endif	/* IO_H */

