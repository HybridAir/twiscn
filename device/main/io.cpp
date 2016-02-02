//input/output stuff

#include "io.h"

LiquidCrystal lcd(7, 8, 13, 10, 11, 12);
uint8_t prevBtn;

void ioInit() {
    //set pin outputs (ports are inputs by default)
    DDRB |= (1<<RED_LED);
    DDRC |= ((1<<CON_LED)|(1<<CONTRAST));
    DDRD |= ((1<<GREEN_LED)|(1<<BLUE_LED));
    
    lcd.begin(16, 2);
    setConLed(true);
    setContrast(true);
    setBacklight(0, 84, 255, 255);
    
    prevBtn = 0;
    
    //lcd start screen
}


void monitorIo() {
    uint8_t currentBtn = getButtons();
    if(currentBtn != prevBtn) {
        prevBtn = currentBtn;
        
        lcd.clear();
        lcd.print(currentBtn, BIN);
    }
    
    
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

    //set the backlight LEDs accordingly
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


//returns the current button states as a byte, 0b000000xx
//0x00 = None, 0x01 = FN1, 0x02 = FN2, 0x03 = FN1 and FN2
uint8_t getButtons() {
    return ((PIND & (1<<FN2_BTN)) | (PINC & (1<<FN1_BTN))) >> 3;
}
