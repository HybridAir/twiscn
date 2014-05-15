//Used for controlling the LCD
//Handles text formatting, display, and scrolling

#include <Arduino.h>
#include <LCD.h>      
#include <avr/pgmspace.h>

#include "TweetHandler.h"

//#include <LiquidCrystal.h>

void LCDControl::LCDControl(Options optin, TweetHandler twtin, byte widthIn) {       //default constructor, wants the options and tweethandler instance, and lcdwidth
    opt = optin;
    twt = twtin;
    LCDWIDTH = widthIn;                                                         //character width of the LCD
    digitalWrite(CONTRASTPIN, HIGH);                                            //enable the LCD's pot contrast power pin, essentially "turning it on"
    lcd.begin(LCDWIDTH, 2);                                                     //get that LCD going
}

void LCDControl::printNewTweet() {                                              //used to print a new tweet
    clearRow(0);                                                                //clear the username row to prepare it for an update
    lcd.print(twt.getUser());                                                   //print the username, don't need to do anything to it 
    printBegin();                                                               //print the beginning of the tweet and do further processing
}

void LCDControl::printBegin() {                                                 //prints the beginning of a tweet, and then enables scrolling if necessary
    section = 0;
    if(twt.useScroll()) {                                                       //check if tweet scrolling is necessary
        scroll = true;
        printedBegin = true;
        previousMillis = millis();
    }
    else {                                                                      //if the tweet is greater than LCDWIDTH, it needs to be scrolled
        scroll = false;
    }
    clearRow(1);                                                                //clear the bottom row first                                              //get the beginning on the tweet
    lcd.print(twt.getTweetBegin());                                             //print the beginning of the tweet
    //call tweet blink here or something
}

void LCDControl::clearRow(byte row) {                                           //used to clear individual rows, give it the row number
    lcd.setCursor(0, row);                                                      //set the row to start clearing
    for(int i = 0;i <= LCDWIDTH; i++) {                                         //for each column in the row
        lcd.print(" ");                                                         //print a space over it, essentially clearing it
    }
    lcd.setCursor(0, row);                                                      //reset cursor position
}

//==============================================================================

void LCDControl::scrollTweet() {                                                //used to scroll the tweet text, must be continuously called
    if(scroll) {
        
        //check if we're at the beginning
            //wait and block other methods here
        //check if we're at the end
            //wait and block other methods here
        //if neither, scroll normally
        
        switch(section) {
            case 0:                                                             //beginning of tweet section
                if(printedBegin) {                                              //already printed the beginning, waiting for the user read time to elapse
                    unsigned long currentMillis = millis();
                    if(currentMillis - previousMillis > readTime) {             //check if it has been readTime yet
                        previousMillis = currentMillis;
                        section++;                                              //done waiting, allow the program to go to the next section
                        lcdPos = 0;
                    }
                }
                else {                                                          //did not print the beginning yet
                    printBegin();
                }
                break;
            case 1:                                                             //scrolling section
                unsigned long currentMillis = millis();
                if(currentMillis - previousMillis > textSpeed) {                //check if it's time to shift the text
                    previousMillis = currentMillis;
                    shiftText();
                    lcdPos++;
                        //section++;                                              //done waiting, allow the program to go to the next section
                }
                break;
        }
    }
}
        
////    if (Row2.length() <= LCD_COLS) {                                            //check if tweet can fit in the bottom row without scrolling first
////        if (newTweet == 1) {                                                    //make sure there is actually a new tweet to display
//            lcd.clear();                                                        //clear the entire lcd, check if this function can clear individual rows
//            displayUser();
//            displayBeginning(Row2.length() - 1);
//            greaterThan16 = 0;
//            newTweet = 0; 
//            timeToBlink = 1;
//        }  
//    }
//    else if (Row2.length() > LCD_COLS) {                                        //jfc where did I go wrong    
//        greaterThan16 = 1;
//        if (newTweet == 0) {   //do this if the text is old, meaning we're scrolling it     
//            if (lcdStart == 0) {
//////                if (waitforbegin == 0) {
//////                    if (unFroze == 0) {
//////                        frozen = 1;
//////                        previousMillis4 = millis();
//////                        waitforbegin = 1;
//////                        firstTweet = 0;
//////                    }
////                    else {
////                        unFroze = 0;
////                    }
////                }
////            }      
//            if (frozen == 0) {
//                if (lcdStart < lcdcount) {    //is there still stuff left to display?
//                    unsigned long currentMillis = millis();
//                    if(currentMillis - previousMillis > finalSpeed) {   //check if it has been long enough between scroll iterations
//                        previousMillis = currentMillis; 
//                        lcdStart++;   //offset the text position by one
//                    }
//                }
//            }
//        }
////        else if (newTweet == 1) {      //display the beginning first, for both lengths 
////            lcd.clear();
////            displayUser();
////            displayBeginning(LCD_COLS); 
////            timeToBlink = 1;  
////            newTweet = 0; 
////            lcdStart = 0;
////        }
//    }
//}

void LCDControl::shiftText() {                                                  //used to shift the tweet text by one space
    if (lcdPos < twt.getTweetLength() - LCDWIDTH) {                           //check if there is remaining text to shift 
        //(subtracted LCDWIDTH since we want the ending to use all the space)
        String subTweet = twt.getTweet();
        subTweet = subTweet.substring(lcdStart, (lcdStart + LCDWIDTH));         //create a substring from the current position to LCDWIDTH chars ahead
        lcd.print(subTweet);
        
        
        
//        for(int i = 0;i < LCDWIDTH;i++) {                                       //for each column in LCDWIDTH
//            lcd.setCursor(i, 1);                                                //set the cursor on the current column in the bottom row
//            String subTweet = twt.getTweet();
//            subTweet = subTweet.substring((lcdStart + i))
//            lcd.print()
//            lcd.print(Row2Array[lcdStart + i]);
//            lcd.print(Row2Array[lcdStart + i]);
//        }
    }
    if(lcdPos == lcdcount) {                                                  //are we at the end of the text?
        section++;
        
//        if (unFroze == 1) {
//            displayBeginning(LCD_COLS);             //get the beginning of the text on the display again
//            lcdStart = 0;    // prepare the scolling function for scrolling this new text
//            unFroze = 0;
//        }
//        else if (frozen == 0) {
//            frozen = 1;
//            unFroze = 0;
//            previousMillis4 = millis();
//        }      
    }
//    else {
//        for(int i = 0; i < LCD_COLS; i++) {   //do this while there is still more text to scroll
//            //print the text
//            lcd.setCursor(i, 1);
//            lcd.print(Row2Array[lcdStart + i]);
//        }
//    }  
}

//void checkfrozen() {
//    if (frozen == 1) {
//        currentMillis4 = millis();
//        if(currentMillis4 - previousMillis4 > freezeTime) {   //do this if it's time to 
//            frozen = 0;
//            unFroze = 1;
//            waitforbegin = 0;
//        }
//    }
//}

void LCDControl::setSpeed(int in) {
    
}




void LCDControl::CreateChar(byte code, PGM_P character) {                       //used to get custom characters out of progmem and into the lcd
    byte* buffer = (byte*)malloc(8);
    memcpy_P(buffer, character,  8);
    lcd.createChar(code, buffer);
    free(buffer);
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

void LCDControl::sleepLCD(bool sleep) {                                         //used to control lcd power state
    if(sleep) {                                                                 //lcd needs to go to sleep
        lcd.clear();                                                            //display a warning message for 4 seconds
        lcd.setCursor(0, 0);
        lcd.print("Host is offline,");
        lcd.setCursor(0, 1);
        lcd.print("entering standby");
        delay(4000);
        lcd.clear();                                                            //clear the display       
        opt.setBrightness(0);                                                   //turn the backlight off
        digitalWrite(LCDPOWPIN, LOW);                                           //turn the contrast off
        lcd.noDisplay();                                                        //turn the rest "off"
    }
    else {                                                                      //lcd needs to wake up
        digitalWrite(LCDPOWPIN, HIGH);                                          //turn the contrast on
        opt.setCol(0, 150, 255);                                                //reset the color
        opt.setBrightness(255);                                                 //turn the backlight on
        lcd.display();                                                          //turn the lcd "on"
        bootAnimation();                                                        //play the boot animation
    }
}





