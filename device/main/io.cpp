//input/output stuff, except for the LCD

#include "io.h"

void ioInit() {
    DDRB |= (1<<RED_LED);                       //the connection LED is an output
    DDRC |= ((1<<CON_LED)|(1<<CONTRAST));             //the connection LED is an output
    //DDRC &= ~(1<<SPEED_POT);
    DDRD |= ((1<<GREEN_LED)|(1<<BLUE_LED));             //the connection LED is an output
    
    setConLed(true);
    setContrast(true);
    setBacklight(0, 84, 255, 255);
}


//sets the state of the green connection LED, give it a boolean value
void setConLed(bool enabled) {
    if(enabled) {
        PORTC |= (1<<CON_LED);
    }
    else {
        PORTC &= ~(1<<CON_LED);
    }
}


//sets the color and brightness of the backlight using PWM
void setBacklight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    r = map(r, 0, 255, 0, brightness);
    g = map(g, 0, 255, 0, brightness);
    b = map(b, 0, 255, 0, brightness);

    //invert values because the backlight LEDs are common-anode
    r = map(r, 0, 255, 255, 0);
    g = map(g, 0, 255, 255, 0);
    b = map(b, 0, 255, 255, 0);

    analogWrite(RED_LED, r);
    analogWrite(GREEN_LED, g);
    analogWrite(BLUE_LED, b);
}


//sets the LCD's contrast state, essentially turning the display on/off
void setContrast(bool enabled) {
    if(enabled) {
        PORTC |= (1<<CONTRAST);
    }
    else {
        PORTC &= ~(1<<CONTRAST);
    }
}


//returns the speed potentiometer's value
uint16_t getSpeed() {
    return analogRead(SPEED_POT);
}

//button checking
