//input/output stuff

#include "io.h"

LiquidCrystal lcd(7, 8, 13, 10, 11, 12);


const byte top1[] = {0x1, 0x1, 0x3, 0x3, 0x7, 0x7, 0x3, 0x1};
const byte top2[] = {0x10, 0x10, 0x18, 0x18, 0x1c, 0x1c, 0x18, 0x10};
const byte left2[] = {0x18, 0x1c, 0x1e, 0x1e, 0x1e, 0x18, 0x0, 0x0};
const byte left1[] = {0x0, 0x0, 0x1, 0x3, 0x7, 0xf, 0x1e, 0x0};
const byte right2[] = {0x3, 0x7, 0xf, 0xf, 0xf, 0x3, 0x0, 0x0};
const byte right1[] = {0x0, 0x0, 0x10, 0x18, 0x1c, 0x1e, 0xf, 0x0};
const char row0[] = "TwiScn v1.1";
const char row1[] = "starting...";


uint8_t prevBtn;
uint16_t prevSpeed;

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
    prevSpeed = 0;
    
    //lcd start screen
    bootScreen();
}


//montior the IO for any changes, and tell the host if necessary
void monitorIo() {
    uint8_t currentBtn = getButtons();                      //get the current button reading
    if(currentBtn != prevBtn) {                             //if the new button reading has changed since the last reading
        prevBtn = currentBtn;                               //save the current reading to the last one
        
        //lcd.clear();
        //lcd.print(currentBtn, BIN);
        //go format the button data and send it to the host
    }
    
    
    //check if the pot changed
    //the host can also just get the current state of the pot too
    
    uint16_t currentSpeed = getSpeed();
    if(currentSpeed != prevSpeed) {                             //if the new button reading has changed since the last reading
        prevSpeed = currentSpeed;                               //save the current reading to the last one
        
        //lcd.clear();
        //lcd.print(currentSpeed, BIN);
        //go format the button data and send it to the host
    }
    
    _delay_ms(100);
    
    //THE DEVICE MUST ASK FOR THIS STUFF
    
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


void printLcdArray(const char string[], char length, char x, char y) {
	lcd.setCursor(x, y);
	for(char i = 0;i < length; i++) {
		lcd.write(string[i]);
	}
}


//fills in an entire row with either empty characters, or filled in characters
void clearRow(uint8_t row, bool isEmpty) {
    lcd.setCursor(0, row);                                                     //set the row to start clearing
    for(uint8_t i = 0;i <= LCDWIDTH; i++) {                                         //for each column in the row
        
        if(isEmpty) {
            lcd.write((uint8_t)254);                                                        //print a space over it, essentially clearing it
        }
        else {
            lcd.write((uint8_t)255);
        }
    }
    lcd.setCursor(0, row);                                                     //reset cursor position
}


//displays a little animation and info on the LCD, usually when the display module is powered on and ready
void bootScreen() {
	logoChars();
	
    clearRow(0, false);
    clearRow(1, false);
	_delay_ms(500);
    lcd.clear();
	_delay_ms(175);
	
	lcd.setCursor(1, 0);
	lcd.write((uint8_t)0);
	lcd.write((uint8_t)1);
	_delay_ms(175);
	
	lcd.setCursor(0, 1);
	lcd.write((uint8_t)3);
	lcd.write((uint8_t)2);
	_delay_ms(175);
	
	lcd.write((uint8_t)4);
	lcd.write((uint8_t)5);
	_delay_ms(175);
	
	printLcdArray(row0, 11, 5, 0);
	printLcdArray(row1, 11, 5, 1);
}


void logoChars() {
	//set the custom logo characters
	lcd.createChar(0, (byte *) top1);
	lcd.createChar(1, (byte *) top2);
	lcd.createChar(2, (byte *) left2);
	lcd.createChar(3, (byte *) left1);
	lcd.createChar(4, (byte *) right2);
	lcd.createChar(5, (byte *) right1);
}