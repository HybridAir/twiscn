#ifndef LCDCONTROL_H
#define	LCDCONTROL_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "Options.h"
#include "TweetHandler.h"

class LCDControl {
    public:
        LCDControl(int widthIn);
        void printNewTweet();
        void printUser();
        void printTweet();
        void scrollTweet();
        void sleepLCD(bool in);
        void prepareLCD();
        void connectAnim();
        void connectDisplay(bool connecting);
        void setSpeed(int in);
    private:
        void CreateChar(byte code, PGM_P character);
        void clearRow(byte row);
        void printBegin();
        void shiftText();
        void bootAnim();
        int LCDWIDTH;    
        int readTime;
        unsigned int textSpeed;
        bool printedBegin;
        bool scroll;
        bool ranOnce;
        bool waitforbegin;
        byte animCount;
        byte section;     
        byte lcdPos;
        unsigned long previousMillis;        
};

#endif	/* LCDCONTROL_H */