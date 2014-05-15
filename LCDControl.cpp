//Used for controlling the LCD
//Handles text formatting, display, scrolling, and backlight control

#include <Arduino.h>
#include <LCD.h>      
#include <avr/pgmspace.h>//too stubborn to move away from their nice library c:
//#include <LiquidCrystal.h>
//#include <PString.h>



#define REDLITE 9    //used to be 3, which is now for usb
#define GREENLITE 5
#define BLUELITE 6
byte lcdStart = 0;      //stores the current position of the scrolling lcd text
byte lcdcount = 0;      //stores the Row2.length() offset
byte count = 0;      //stores the connection animation thing

static prog_char PROGMEM top1[] = {0x1,0x1,0x3,0x3,0x7,0x7,0x3,0x1};
static prog_char PROGMEM top2[] = {0x10,0x10,0x18,0x18,0x1c,0x1c,0x18,0x10};
static prog_char PROGMEM left2[] = {0x8,0x1c,0x1e,0x1e,0x1e,0x18,0x0,0x0};
static prog_char PROGMEM left1[] = {0x0,0x0,0x0,0x1,0x3,0x7,0xf,0x18};
static prog_char PROGMEM right2[] = {0x2,0x7,0xf,0xf,0xf,0x3,0x0,0x0};
static prog_char PROGMEM right1[] = {0x0,0x0,0x0,0x10,0x18,0x1c,0x1e,0x3};

unsigned int finalSpeed = 0;   //converted speed value taken from the speed potentiometer
unsigned int interval2 = 3000;   //how long the lcd is kept frozen for
unsigned int freezeTime = 2000;


boolean frozen = 0;   //stores if the lcd is currently frozen
boolean beginning = 0;   //stores if the text on the lcd is at the beginning
boolean waitforbegin = 0;   //stores if we are waiting for the beginning of the text
boolean unFroze = 0;

int howLongItsBeen;   //how long it has been since the color level was changed
int rain = 0;   //used for changing the color level
int color1 = 1;
int color2 = 0;
int color3 = 0;


  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT); 




LCDControl::LCDControl() {                                            //Constructor, wants the options object probably created in main.cpp
    //opt = optin;
    //twt = twtin;
    color = opt.getColor();                                                      //get the color var
    brightness = opt.getBright();                                                //and brightness var from the options class (not yet made)
    //digitalWrite(CONTRASTPIN, HIGH);                                            //enable the LCD's pot contrast power pin, essentially "turning it on"
    //turns out there are some things called noDisplay() and display(), never even needed the contrast pin (kill me c:)
    lcd.begin(LCDWIDTH,2);                                                      //get that LCD going
    //bootAnim();
}

void LCDControl::printUser(String in) {                                         //prints the tweet's username, wants a String
    clearRow(0);                                                                //clear the username row to prepare it for an update
    lcd.print(in);                                                              //print the username, don't need to do anything to it
}

void LCDControl::printTweet() {                                                 //used to print a new tweet, and then handle scrolling
    printBegin();
//scrolling stuff here

}

void LCDControl::printBegin() {                                                 //used to print the beginning of the tweet, wants the tweet
    clearRow(1);                                                                //clear the bottom row first
    String out = twt.getTwtBegin();                                               //get the beginning on the tweet
    lcd.print(out);                                                             //since it's going to be LCDWIDTH or less, don't do anything to it and print it 
}

void LCDControl::clearRow(byte row) {                                           //used to clear individual rows, give it the row number
    lcd.setCursor(0, row);                                                      //set the row to start clearing
    for(int i = 0;i <= LCDWIDTH; i++) {                                         //for each column in the row
        lcd.print(" ");                                                         //print a space over it, essentially clearing it
    }
    lcd.setCursor(0, row);                                                      //reset cursor position
}









void LCDControl::CreateChar(byte code, PGM_P character) {                       //used to get customs characters out of progmem and into the lcd
    byte* buffer = (byte*)malloc(8);
    memcpy_P(buffer, character,  8);
    lcd.createChar(code, buffer);
    free(buffer);
}









void LCDControl::scrolltext() {                                                 //used to scroll the text on the bottom row
    if (Row2.length() <= LCD_COLS) {                                            //check if tweet can fit in the bottom row without scrolling first
        if (newTweet == 1) {                                                    //make sure there is actually a new tweet to display
            lcd.clear();                                                        //clear the entire lcd, check if this function can clear individual rows
            displayUser();
            displayBeginning(Row2.length() - 1);
            greaterThan16 = 0;
            newTweet = 0; 
            timeToBlink = 1;
        }  
    }
    else if (Row2.length() > LCD_COLS) {                                        //jfc where did I go wrong    
        greaterThan16 = 1;
        if (newTweet == 0) {   //do this if the text is old, meaning we're scrolling it     
            if (lcdStart == 0) {
                if (waitforbegin == 0) {
                    if (unFroze == 0) {
                        frozen = 1;
                        previousMillis4 = millis();
                        waitforbegin = 1;
                        firstTweet = 0;
                    }
                    else {
                        unFroze = 0;
                    }
                }
            }      
            if (frozen == 0) {
                if (lcdStart < lcdcount) {    //is there still stuff left to display?
                    unsigned long currentMillis = millis();
                    if(currentMillis - previousMillis > finalSpeed) {   //check if it has been long enough between scroll iterations
                        previousMillis = currentMillis; 
                        lcdStart++;   //offset the text position by one
                    }
                }
            }
        }
        else if (newTweet == 1) {      //display the beginning first, for both lengths 
            lcd.clear();
            displayUser();
            displayBeginning(LCD_COLS); 
            timeToBlink = 1;  
            newTweet = 0; 
            lcdStart = 0;
        }
    }
}

void printtext() {   //used to printing the tweet to the display
    if(lcdStart == lcdcount) {    //are we at the end of the text?
        if (unFroze == 1) {
            displayBeginning(LCD_COLS);             //get the beginning of the text on the display again
            lcdStart = 0;    // prepare the scolling function for scrolling this new text
            unFroze = 0;
        }
        else if (frozen == 0) {
            frozen = 1;
            unFroze = 0;
            previousMillis4 = millis();
        }      
    }
    else {
        for(int i = 0; i < LCD_COLS; i++) {   //do this while there is still more text to scroll
            //print the text
            lcd.setCursor(i, 1);
            lcd.print(Row2Array[lcdStart + i]);
        }
    }  
}

void checkfrozen() {
    if (frozen == 1) {
        currentMillis4 = millis();
        if(currentMillis4 - previousMillis4 > freezeTime) {   //do this if it's time to 
            frozen = 0;
            unFroze = 1;
            waitforbegin = 0;
        }
    }
}






void LCDControl::bootAnim() {                                                   //simple boot animation, needs to be called after the customs chars are made
    setBacklight(r, g, b);
    delay(25);
    brightness = 250;
    setBacklight(r, g, b);
    delay(25);
    brightness = 0;
    setBacklight(r, g, b);
    delay(25);
    brightness = 250;
    setBacklight(r, g, b);
    delay(25);
    brightness = 0;
    setBacklight(r, g, b);
    delay(200);
    brightness = 250;
    setBacklight(r, g, b); 

    CreateChar(0, top1);
    CreateChar(1, top2);
    CreateChar(2, left1);
    CreateChar(3, left2);
    CreateChar(4, right2);
    CreateChar(5, right1);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.print((char)0);
    lcd.print((char)1);
    delay(250);
    lcd.setCursor(0, 1);
    lcd.print((char)2);
    lcd.print((char)3);
    delay(250);
    lcd.print((char)4);
    lcd.print((char)5);
    delay(250); 
    lcd.setCursor(6, 0);
    lcd.print("Twitter");
    lcd.setCursor(6, 1);
    lcd.print("Screen v1"); 
    delay(1000);
    lcd.setCursor(6, 0);
    lcd.print("Waiting");
    lcd.setCursor(6, 1);
    lcd.print("for USB  ");
}

void LCDControl::connecting() {                                                 //animation displayed while the device is connecting
    while(runOnce == 0) {
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("Connecting");
        lcd.setCursor(6, 1);
        lcd.print("to Host");
        runOnce = 1;
        unsigned long previousMillis = 0; 
    }

    if (count == 3) {
        count = 0;
    }
    if (count == 0) {
        lcd.setCursor(0, 1);
        lcd.print("    ");
        lcd.setCursor(1, 0);
        lcd.print((char)0);
        lcd.print((char)1);
    }
    else if (count == 1) {
        lcd.setCursor(0, 0);
        lcd.print("   ");
        lcd.setCursor(0, 1);
        lcd.print((char)2);
        lcd.print((char)3);
    }
    else if (count == 2) {
        lcd.setCursor(0, 1);
        lcd.print("  ");
        lcd.print((char)4);
        lcd.print((char)5);
    }
    count++;
}

void LCDControl::connected() {                                                  //shows after the device is connected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("latest data...");
}

void LCDControl::sleepLCD(bool in) {
    if(in) {                                                                    //lcd needs to go to sleep
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Host is offline,");
        lcd.setCursor(0, 1);
        lcd.print("entering standby");
        delay(4000);
        lcd.clear();
        brightness = 0;
        setBacklight(r, g, b);
        digitalWrite(LCDPOWPIN, LOW);
        lcd.noDisplay();
    }
    else {                                                                      //lcd needs to wake up
        digitalWrite(LCDPOWPIN, HIGH);
        lcd.display();
//        brightness = 0;
//        setBacklight(r, g, b);
        bootAnimation();
    }
}





void LCDControl::setSpeed(int in) {
    
}