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
    readTime = 3000;                                                            //set the time for the user to read the beginning and end of a tweet, put this in options later
    ranOnce = false;                                                            //used in connectDisplay
    animCount = 0;                                                              //used in connectAnim
    previousMillis = 0;                                                         //used in printBegin
    lcdPos = 0;                                                                 //stores the current position of the scrolling lcd text
    textSpeed = 0;                                                              //final speed value taken from the speed potentiometer
    waitforbegin = 0;                                                           //stores if we are waiting for the beginning of the text
}

void LCDControl::printNewTweet() {                                              //used to print a new tweet
    clearRow(0);                                                                //clear the username row to prepare it for an update
    opt.setReadyBlink(true);                                                    //tell options that we need to blink the backlight
    lcdc.print(twt.getUser());                                                   //print the username, don't need to do anything to it 
    printBegin();                                                               //print the beginning of the tweet and do further processing
}

void LCDControl::printBegin() {                                                 //prints the beginning of a tweet, and then enables scrolling if necessary
    section = 0;                                                                //let the scrolltext method know to start at section 0
    if(twt.useScroll()) {                                                       //ask tweethandler if scrolling is necessary, true for this case
        scroll = true;                                                          //enable scrolling
        printedBegin = true;                                                    //let the program know the beginning was already printed
        previousMillis = millis();                                              //set previousMillis to the current time, needed for new tweets made after this one
    }
    else {                                                                      //if scrolling is not necessary
        scroll = false;                                                         //disable scrolling
    }
    clearRow(1);                                                                //clear the bottom row
    lcdc.print(twt.getTweetBegin());                                            //print the beginning of the tweet
    
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
                    if(currentMillis1 - previousMillis > readTime) {            //wait for the user read time to elapse
                        previousMillis = currentMillis1;
                        section++;                                              //done waiting, allow the program to go to the next section
                        lcdPos = 0;                                             //reset the lcdPos var, needs to start at 0 after the beginning
                    }
                }
                else {                                                          //did not print the beginning yet
                    printBegin();                                               //print the beginning already
                }
                break;
            }
            case 1:  {                                                          //scrolling section
                unsigned long currentMillis2 = millis();
                if(currentMillis2 - previousMillis > textSpeed) {               //check if it's time to shift the text
                    previousMillis = currentMillis2;
                    shiftText();                                                //shift the text by one                                                  
                }
                break;
            }
            case 2:   {                                                         //end of tweet section
                unsigned long currentMillis3 = millis();
                if(currentMillis3 - previousMillis > readTime) {                //wait for the user read time to elapse
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
    if (lcdPos <= (twt.getTweetLength() - LCDWIDTH)) {                            
        //(subtracted LCDWIDTH since we want the ending to use all of LCDWIDTH)
        String subTweet = twt.getTweet();
        subTweet = subTweet.substring(lcdPos, (lcdPos + LCDWIDTH));             //create a substring from the current position to LCDWIDTH chars ahead
        lcdc.setCursor(0, 1);                                                   //make sure we print on the bottom row
        lcdc.print(subTweet);                                                   //printed the shifted substring
        lcdPos++;                                                               //increase lcdPos by one
    }
    if(lcdPos == ((twt.getTweetLength() - LCDWIDTH)+1)) {                       //check if we are at the end of the text to be shifted
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
        delay(5);
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
    lcdc.print("Twitter");
    lcdc.setCursor(6, 1);
    lcdc.print("Screen v1"); 
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
            delay(5);
        }
        lcdc.clear();                                                           //clear the display       
        lcdc.noDisplay();                                                       //turn the lcd "off"
    }
    else {                                                                      //lcd needs to wake up
        lcdc.display();                                                         //turn the lcd "on"     
        bootAnim();                                                             //play the boot animation
    }
}