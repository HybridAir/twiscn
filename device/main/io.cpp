//input/output stuff, except for the LCD

#include "io.h"

void ioInit() {
    DDRC |= (1<<CONN_LED);             //the connection LED is an output
    _delay_ms(1000);                    //used for testing
}