#ifndef LCDCONTROL_H
#define	LCDCONTROL_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LCDControl {
    public:
        LCDControl(Options in);
        printNewTweet(String userin, String twtBeg, String twtin);
        printNewTweet(String userin, String twtin);
        printUser();
        printTweet();
        scrollTweet();
        sleepLCD(bool in);
        prepareLCD();
        connectAnim(bool connecting);
        setSpeed(int in);
    private:
        LiquidCrystal lcd(7, 8, 13, 10, 11, 12);                                    //create a new instance of LiquidCrystal, with these pins
                                                        //character width of the LCD
        CreateChar(byte code, PGM_P character);
        clearRow(byte row);
        printBegin(String in);
        shiftText();
        bootAnim();
        Options opt;
        TweetHandler twt;
        const int LCDWIDTH;
        
        const int readTime = 4000;
        byte section;
        bool printedBegin;
        bool scroll;
        bool ranOnce = false;
        byte animCount = 0;
        unsigned long previousMillis = 0;
        //int brightness= 0;
        const int CONTRASTPIN = 16;                                               //pin used to supply power to the contrast pot
        static prog_char PROGMEM top1[] = {0x1,0x1,0x3,0x3,0x7,0x7,0x3,0x1};
        static prog_char PROGMEM top2[] = {0x10,0x10,0x18,0x18,0x1c,0x1c,0x18,0x10};
        static prog_char PROGMEM left2[] = {0x8,0x1c,0x1e,0x1e,0x1e,0x18,0x0,0x0};
        static prog_char PROGMEM left1[] = {0x0,0x0,0x0,0x1,0x3,0x7,0xf,0x18};
        static prog_char PROGMEM right2[] = {0x2,0x7,0xf,0xf,0xf,0x3,0x0,0x0};
        static prog_char PROGMEM right1[] = {0x0,0x0,0x0,0x10,0x18,0x1c,0x1e,0x3};
        byte lcdPos = 0;      //stores the current position of the scrolling lcd text
        byte lcdcount = 0;      //stores the Row2.length() offset
        byte count = 0;      //stores the connection animation thing
        unsigned int textSpeed = 0;   //converted speed value taken from the speed potentiometer
        unsigned int interval2 = 3000;   //how long the lcd is kept frozen for
        unsigned int freezeTime = 2000;
        boolean frozen = 0;   //stores if the lcd is currently frozen
        boolean beginning = 0;   //stores if the text on the lcd is at the beginning
        boolean waitforbegin = 0;   //stores if we are waiting for the beginning of the text
        boolean unFroze = 0;
};

#endif	/* LCDCONTROL_H */

