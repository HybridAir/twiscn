#include <avr/io.h>
#include <util/delay.h>

#include <Arduino.h>
#include <LiquidCrystal.h>  

#include "io.h"

LiquidCrystal lcd(7, 8, 13, 10, 11, 12);

int main(void)
{
    ioInit();
    
    PORTB &= ~(1<<RED_LED);
    PORTD &= ~((1<<GREEN_LED)|(1<<BLUE_LED));
    
    PORTC |= (1<<CONTRAST);

    
    lcd.begin(16, 2);
    lcd.print("I got it working");
    lcd.setCursor(0,1);
    l.print("again");

	while(1)
	{
		PORTC |= (1<<CONN_LED);
		_delay_ms(100);

		PORTC &= ~(1<<CONN_LED);
		_delay_ms(100);
	}

	return 0;
}