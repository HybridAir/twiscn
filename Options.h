#ifndef OPTIONS_H
#define	OPTIONS_H

#include <Arduino.h>
#include "IO.h"

class Options {
    public:
        Options();
        //void begin(IO& ioin);
        byte getBrightness();
        byte *getCol();
        byte *getBlinkCol();
        bool getRainbow();
        int getRainSpd();
        bool getBlink();
        byte getBlinkSpd();
        bool getReadyBlink();
        void setBrightness(byte in);
        void setCol(byte r, byte g, byte b);
        void setBlinkCol(byte r, byte g, byte b);
        void setRainbow(bool in);
        void setRainSpd(int in);
        void setBlink(bool in);
        void setBlinkSpd(byte in);
        void setReadyBlink(bool in);
        void extractOption(String in);
        void updateCol();
        void updateBlinkCol();
    private:
        void getBrightnessVal(String in);
        void getColorVal(String in);
        void getTweetBlink(String in);
        void getRainbow(String in);
        //IO inout;
        byte brightness;
        bool rainbow;
        int rainSpd;                                                            //int to support very long color changing times
        bool blink;
        byte blinkSpd;                                                          //byte since the blink speed should be fast, you know
        bool readyBlink;
};

#endif	/* OPTIONS_H */

