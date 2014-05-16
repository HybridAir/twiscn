#ifndef LCDCONTROL_H
#define	LCDCONTROL_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>

class LCDControl {
    public:
        LCDControl();
        void printNewTweet(String userin, String twtBeg, String twtin);
        void printNewTweet(String userin, String twtin);
        void printUser();
        void printTweet();
        void scrollTweet();
        void sleepLCD(bool in);
        void prepareLCD();
        void connectAnim(bool connecting);
        void setSpeed(int in);
    private:
        LiquidCrystal lcdc;                                    //create a new instance of LiquidCrystal, with these pins
                                                        //character width of the LCD
        void CreateChar(byte code, PGM_P character);
        void clearRow(byte row);
        void printBegin(String in);
        void shiftText();
        void bootAnim();
        //Options opt;
        //TweetHandler twt;
        const int LCDWIDTH;
        
        const int readTime;
        byte section;
        bool printedBegin;
        bool scroll;
        bool ranOnce;
        byte animCount;
        unsigned long previousMillis;
        //int brightness= 0;
        const int CONTRASTPIN;                                               //pin used to supply power to the contrast pot
        static prog_char PROGMEM top1[];
        static prog_char PROGMEM top2[];
        static prog_char PROGMEM left2[];
        static prog_char PROGMEM left1[];
        static prog_char PROGMEM right2[];
        static prog_char PROGMEM right1[];
        byte lcdPos;      //stores the current position of the scrolling lcd text
        byte lcdcount;      //stores the Row2.length() offset
        byte count;      //stores the connection animation thing
        unsigned int textSpeed;   //converted speed value taken from the speed potentiometer
        unsigned int interval2;   //how long the lcd is kept frozen for
        unsigned int freezeTime;
        boolean frozen;   //stores if the lcd is currently frozen
        boolean beginning;   //stores if the text on the lcd is at the beginning
        boolean waitforbegin;   //stores if we are waiting for the beginning of the text
        boolean unFroze;
};

#endif	/* LCDCONTROL_H */

