#include <Arduino.h>
//#include <avr/io.h>
//#include <util/delay.h>

//#define F_CPU 16000000UL

#include <LiquidCrystal.h>  

#include "io.h"

LiquidCrystal lcd(7, 8, 13, 10, 11, 12);

int main(void)
{
    init();
    ioInit();
    lcd.begin(16, 2);
    
	while(1)
	{
        lcd.setCursor(0,0);
        lcd.print(getButtons());
	}

	return 0;
}