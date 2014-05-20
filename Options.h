#ifndef OPTIONS_H
#define	OPTIONS_H

#include <Arduino.h>
#include "IO.h"
#include "LCDControl.h"
#include "TweetHandler.h"

class Options {
    public:
        Options();   
        byte getBrightness();
        byte *getCol();
        byte *getBlinkCol();
        byte getBlinkSpd();
        bool getRainbow();
        bool getBlink(); 
        bool getReadyBlink();
        int getRainSpd();
        int getReadTime();
        void defaults();
        void setBrightness(byte in);
        void setCol(byte r, byte g, byte b);
        void setBlinkCol(byte r, byte g, byte b);
        void setRainbow(bool in);
        void setRainSpd(int in);
        void setBlink(bool in);
        void setBlinkSpd(byte in);
        void setReadyBlink(bool in);
        void setReadTime(int in);
        void extractOption(String in);
        void updateCol();
        void updateBlinkCol();
    private:
        void getBrightnessVal(String in);
        void getColorVal(String in);
        void getTweetBlink(String in);
        void getRainbow(String in);
        void getReadTimeVal(String in);
        void getPrevTweet(String in);
        byte color[3];                                                    
        byte blinkColor[3]; 
        byte brightness;
        byte blinkSpd;                                                         
        bool rainbow;
        bool blink;
        bool readyBlink;
        bool onPrevious;
        unsigned int readTime;
        unsigned int rainSpd;                                                   
};

#endif	/* OPTIONS_H */

