#ifndef OPTIONS_H
#define	OPTIONS_H

#include <Arduino.h>

class Options {
    public:
        Options();
        getBrightness();
        getCol();
        getBlinkCol();
        getRainbow();
        getRainSpd();
        getBlink();
        getBlinkSpd();
        getReadyBlink();
        setBrightness(byte in);
        setCol(byte r, byte g, byte b);
        setBlinkCol(byte r, byte g, byte b);
        setRainbow(bool in);
        setRainSpd(int in);
        setBlink(bool in);
        setBlinkSpd(byte in);
        setReadyBlink(bool in);
        extractOption(String in);
        updateCol();
        updateBlinkCol();
    private:
        getBrightnessVal(String in);
        getColorVal(String in);
        getTweetBlink(String in);
        getRainbow(String in);
        IO inout;
        byte brightness;
        byte color[3];
        byte blinkColor[3];
        bool rainbow;
        int rainSpd;                                                            //int to support very long color changing times
        bool blink;
        byte blinkSpd;                                                          //byte since the blink speed should be fast, you know
        bool readyBlink = false;
};

#endif	/* OPTIONS_H */

