//Handles text formatting, display, and scrolling
 
#include "LCDControl.h"

extern Options opt;
extern TweetHandler twt;
extern LiquidCrystal lcdc;

//custom lcd characters for the logo
static prog_char PROGMEM top1[] = {0x1,0x1,0x3,0x3,0x7,0x7,0x3,0x1};
static prog_char PROGMEM top2[] = {0x10,0x10,0x18,0x18,0x1c,0x1c,0x18,0x10};
static prog_char PROGMEM left2[] = {0x8,0x1c,0x1e,0x1e,0x1e,0x18,0x0,0x0};
static prog_char PROGMEM left1[] = {0x0,0x0,0x0,0x1,0x3,0x7,0xf,0x18};
static prog_char PROGMEM right2[] = {0x2,0x7,0xf,0xf,0xf,0x3,0x0,0x0};
static prog_char PROGMEM right1[] = {0x0,0x0,0x0,0x10,0x18,0x1c,0x1e,0x3};

LCDControl::LCDControl(int widthIn) {                                           //constructor, wants the lcdwidth  
    LCDWIDTH = widthIn;                                                         //character width of the LCD
    lcdc.begin(LCDWIDTH, 2);                                                    //get that LCD going  
    ranOnce = false;                                                            //used in connectDisplay
    animCount = 0;                                                              //used in connectAnim
    previousMillis = 0;                                                         //used in printBegin
    lcdPos = 0;                                                                 //stores the current position of the scrolling lcd text
    textSpeed = 0;                                                              //final speed value taken from the speed potentiometer
    waitforbegin = 0;                                                           //stores if we are waiting for the beginning of the text
}

void LCDControl::printNewTweet(bool current) {                                  //used to print a new tweet, needs to know if this is the current tweet or not
    clearRow(0);                                                                //clear the username row to prepare it for an update
    opt.setReadyBlink(true);                                                    //trigger a tweetblink, if enabled
    if(current) {                                                               //if we are on the current tweet
        currentTweet = true;                                                    //let the rest of the class know
        lcdc.print(twt.getUser());                                              //print the username, don't need to do anything to it 
        printBegin(twt.getTweetBegin());                                        //print the beginning of the tweet and do further processing, give it the beginning
    }
    else {                                                                      //if we are on the previous tweet
        currentTweet = false;                                                   //let the rest of the class know
        lcdc.print(twt.getPrevUser());                                          //print the previous username, don't need to do anything to it
        printBegin(twt.getPrevBegin());                                         //print the beginning of the tweet and do further processing, give it the previous beginning
    }
}

void LCDControl::printBegin(String begin) {                                     //prints the beginning of a tweet, and then enables scrolling if necessary
    section = 0;                                                                //let the scrolltext method know to start at section 0
    if(twt.useScroll(currentTweet)) {                                           //ask tweethandler if scrolling is necessary
        scroll = true;                                                          //enable scrolling
        printedBegin = true;                                                    //let the program know the beginning was already printed
        previousMillis = millis();                                              //set previousMillis to the current time, needed for new tweets made after this one
    }
    else {                                                                      //if scrolling is not necessary
        scroll = false;                                                         //disable scrolling
    }
    clearRow(1);                                                                //clear the bottom row
    lcdc.print(begin);                                            //print the beginning of the tweet
    
}

void LCDControl::clearRow(byte row) {                                           //used to clear individual rows, give it the row number
    lcdc.setCursor(0, row);                                                     //set the row to start clearing
    for(int i = 0;i <= LCDWIDTH; i++) {                                         //for each column in the row
        lcdc.print(" ");                                                        //print a space over it, essentially clearing it
    }
    lcdc.setCursor(0, row);                                                     //reset cursor position
}

//==============================================================================

void LCDControl::scrollTweet() {                                                //used to scroll the tweet text, must be continuously called
    if(scroll) {
        switch(section) {
            case 0: {                                                           //beginning of tweet section
                if(printedBegin) {                                              //if we already printed the beginning
                    unsigned long currentMillis1 = millis();
                    if(currentMillis1 - previousMillis > opt.getReadTime()) {   //wait for the user read time to elapse
                        previousMillis = currentMillis1;
                        section++;                                              //done waiting, allow the program to go to the next section
                        lcdPos = 0;                                             //reset the lcdPos var, needs to start at 0 after the beginning
                    }
                }
                else {                                                          //did not print the beginning yet
                    if(currentTweet) {                                          //if we are on the current tweet
                        printBegin(twt.getTweetBegin());                        //print the beginning
                    }
                    else {                                                      //if we are on the previous tweet
                        printBegin(twt.getPrevBegin());                         //print the previous beginning
                    }
                }
                break;
            }
            case 1:  {                                                          //scrolling section
                if(opt.getScroll()) {
                    unsigned long currentMillis2 = millis();
                    if(currentMillis2 - previousMillis > textSpeed) {               //check if it's time to shift the text
                        previousMillis = currentMillis2;
                        shiftText();                                                //shift the text by one                                                  
                    }
                }
                break;
            }
            case 2:   {                                                         //end of tweet section
                unsigned long currentMillis3 = millis();
                if(currentMillis3 - previousMillis > opt.getReadTime()) {       //wait for the user read time to elapse
                    previousMillis = currentMillis3;
                    section = 0;                                                //done waiting, go back to section 0
                    printedBegin = false;
                }
                break;
            }
        }
    }
}

void LCDControl::shiftText() {                                                  //used to shift the tweet text by one column
    if(currentTweet) {                                                          //if we are on the current tweet
        twtLength = twt.getTweetLength();                                       //save the tweet length
    }
    else {                                                                      //if we are on the previous tweet
        twtLength = twt.getPrevLength();                                        //save the previous tweet length
    }
    if (lcdPos <= (twtLength - LCDWIDTH)) {                            
        //(subtracted LCDWIDTH since we want the ending to use all of LCDWIDTH)
        if(currentTweet) {                                                      //get the current tweet
            subTweet = twt.getTweet();
        }
        else {                                                                  //or get the previous tweet
            subTweet = twt.getPrevTweet();
        }
        subTweet = subTweet.substring(lcdPos, (lcdPos + LCDWIDTH));             //create a substring from the current position to LCDWIDTH chars ahead
        lcdc.setCursor(0, 1);                                                   //make sure we print on the bottom row
        lcdc.print(subTweet);                                                   //printed the shifted substring
        lcdPos++;                                                               //increase lcdPos by one
    }
    if(lcdPos == ((twtLength - LCDWIDTH)+1)) {                                  //check if we are at the end of the text to be shifted
        section++;                                                              //we are done here, go to the next section
    }
}

void LCDControl::setSpeed(int in) {                                             //used to set the text shifting speed
    textSpeed = in;
}

//==============================================================================

void LCDControl::CreateChar(byte code, PGM_P character) {                       //used to get custom characters out of progmem and into the lcd
    byte* buffer = (byte*)malloc(8);
    memcpy_P(buffer, character,  8);
    lcdc.createChar(code, buffer);
    free(buffer);
}

void LCDControl::bootAnim() {                                                   //simple boot animation, needs to be called after the custom chars are made
    for(int i = 0; i <= 255; i++) {                                             //fades the backlight on
        opt.setBrightness(i);
        delay(2);
    } 
    //create each custom character for use
    CreateChar(0, top1);
    CreateChar(1, top2);
    CreateChar(2, left1);
    CreateChar(3, left2);
    CreateChar(4, right2);
    CreateChar(5, right1);

    lcdc.clear();
    lcdc.setCursor(0, 0);
    lcdc.print(" ");
    lcdc.print((char)0);
    lcdc.print((char)1);
    delay(250);
    lcdc.setCursor(0, 1);
    lcdc.print((char)2);
    lcdc.print((char)3);
    delay(250);
    lcdc.print((char)4);
    lcdc.print((char)5);
    delay(250); 
    lcdc.setCursor(6, 0);
    lcdc.print("TwiScn");
    lcdc.setCursor(6, 1);
    lcdc.print("Version 1a"); 
    delay(2000);
    lcdc.setCursor(6, 0);
    lcdc.print("Waiting");
    lcdc.setCursor(6, 1);
    lcdc.print("for USB  ");
}

void LCDControl::connectAnim() {                                                //displays a connecting animation on the lcd, must be called to advance "frames"
    if(animCount == 2) {
        animCount = 0;
    }
    else {
        animCount++;
    }
    switch(animCount) {
    case 0:
        lcdc.setCursor(0, 1);
        lcdc.print("    ");
        lcdc.setCursor(1, 0);
        lcdc.print((char)0);
        lcdc.print((char)1);
        break;
    case 1:
        lcdc.setCursor(0, 0);
        lcdc.print("   ");
        lcdc.setCursor(0, 1);
        lcdc.print((char)2);
        lcdc.print((char)3);
        break;
    case 2:
        lcdc.setCursor(0, 1);
        lcdc.print("  ");
        lcdc.print((char)4);
        lcdc.print((char)5);
        break;
    }
}

void LCDControl::connectDisplay(bool connecting) {                              //displays a different message depending on if the device is connected or not
    if(connecting) {                                                            //if we are connecting, display the following message only once
        if(!ranOnce) {
            lcdc.clear();
            lcdc.setCursor(6, 0);
            lcdc.print("Connecting");
            lcdc.setCursor(6, 1);
            lcdc.print("to Host");
            ranOnce = true;                                                     //don't run this again
            animCount = 0;                                                      //reset the animCount in connectAnim
        }
    }
    else {                                                                      //if we just finished connecting:
        lcdc.clear();
        lcdc.setCursor(0, 0);
        lcdc.print("Waiting for");
        lcdc.setCursor(0, 1);
        lcdc.print("latest data...");
    }
}

void LCDControl::sleepLCD(bool sleep) {                                         //used to control lcd power state
    if(sleep) {                                                                 //if the lcd needs to go to sleep
        lcdc.clear();                                                           //display a warning message for 4 seconds
        lcdc.setCursor(0, 0);
        lcdc.print("Host is offline,");
        lcdc.setCursor(0, 1);
        lcdc.print("entering standby");
        delay(4000);  
        scroll = false;                                                         //no longer need to scroll
        byte b = opt.getBrightness();                                           //get the current brightness to reference later
        for(int i = b; i >= 0; i--) {                                           //fade out the backlight
            opt.setBrightness(i);
            delay(2);
        }
        lcdc.clear();                                                           //clear the display       
        lcdc.noDisplay();                                                       //turn the lcd "off"
    }
    else {                                                                      //lcd needs to wake up
        lcdc.display();                                                         //turn the lcd "on"     
        bootAnim();                                                             //play the boot animation
    }
}

void LCDControl::scrollNotification(boolean paused) {                           //used to display the "scrolling paused" notification, needs the scroll status
    if(paused) {                                                                //if scrolling was paused
        clearRow(0);                                                            //clear the top row
        lcdc.print("[Scroll  Paused]");                                         //display the notice
    }
    else {                                                                      //if scrolling was unpaused
        if(twt.getUser() != "") {                                               //and also if we got the username
            //needed when all options are set before the first tweet gets here
            clearRow(0);                                                        //clear the top row
            lcdc.print(twt.getUser());                                          //print the username
        }
    }
    
}