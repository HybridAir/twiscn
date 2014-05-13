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
    private:
        byte brightness;
        byte color[3];
        byte blinkColor[3];
        bool rainbow;
        int rainSpd;                                                            //int to support very long color changing times
        bool blink;
        byte blinkSpd;                                                          //byte since the blink speed should be fast, you know
};

#endif	/* OPTIONS_H */

