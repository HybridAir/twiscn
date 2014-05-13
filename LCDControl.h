#ifndef LCDCONTROL_H
#define	LCDCONTROL_H

#include <Arduino.h>

class LCDControl {
    public:
        LCDControl(Options in);
        printUser();
        void prepareLCD();
        const int LCDWIDTH = 16;                                                //character width of the LCD
    private:
        LiquidCrystal lcd(7, 8, 13, 10, 11, 12);                                    //create a new instance of LiquidCrystal, with these pins
        CreateChar(byte code, PGM_P character);
        clearRow(byte row);
        int brightness= 0;
        Color color;
        const int CONTRASTPIN = 16;                                               //pin used to supply power to the contrast pot
};

#endif	/* LCDCONTROL_H */

