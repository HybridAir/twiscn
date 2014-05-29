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
        void printNewTweet(bool current);
        void printUser();
        void printTweet();
        void scrollTweet();
        void sleepLCD(bool in);
        void prepareLCD();
        void connectAnim();
        void connectDisplay(bool connecting);
        void setSpeed(int in);
        void scrollNotification(boolean paused);
    private:
        void CreateChar(byte code, PGM_P character);
        void clearRow(byte row);
        void printBegin(String begin);
        void shiftText();
        void bootAnim();
        String subTweet;
        byte LCDWIDTH;    
        unsigned int textSpeed;
        bool printedBegin;
        bool scroll;
        bool ranOnce;
        bool waitforbegin;
        bool currentTweet;
        byte animCount;
        byte section;     
        byte lcdPos;
        unsigned long previousMillis; 
        int twtLength;
};

#endif	/* LCDCONTROL_H */