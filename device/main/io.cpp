//input/output stuff, except for the LCD

#include "io.h"

void ioInit() {
    DDRB |= (1<<RED_LED);             //the connection LED is an output
    DDRC |= ((1<<CONN_LED)|(1<<CONTRAST));             //the connection LED is an output
    DDRD |= ((1<<GREEN_LED)|(1<<BLUE_LED));             //the connection LED is an output
    
    
    _delay_ms(1000);                    //used for testing
}