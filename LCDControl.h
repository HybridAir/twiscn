#ifndef LCDCONTROL_H
#define	LCDCONTROL_H

#include <Arduino.h>

class LCDControl {
    public:
        LCDControl(Options in);
        void prepareLCD();
    private:
        CreateChar(byte code, PGM_P character);
};

#endif	/* LCDCONTROL_H */

