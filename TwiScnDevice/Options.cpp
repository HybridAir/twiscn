//handles device options/settings
#include "Options.h"

extern IO inout;                                                                //needed for IO class access
extern LCDControl lcd;
extern TweetHandler twt;

Options::Options() {                                                            //default constructor, sets up default options
    defaults();
}

void Options::defaults() {
    brightness = 0;                                                             //LCD backlight brightness
    setCol(0, 150, 255);                                                        //LCD backlight color
    setBlinkCol(255, 0, 0);                                                     //LCD backlight blink color
    rainbow = false;                                                            //rainbow LCD mode
    rainSpd = 200;                                                              //rainbow color transition speed
    blink = false;                                                              //new tweet blink mode
    blinkSpd = 100;                                                             //tweet blink speed
    readyBlink = false;                                                         //stores if we are ready to blink
    readTime = 4000;                                                            //time to wait for the user to read the beginning/end of a tweet
    onPrevious = false;
    scroll = true;
    sleep = false;
}

//==============================================================================

byte Options::getBrightness() {
    return brightness;
}

byte *Options::getCol() {                                                       //returns a pointer to the color array
    return color;
}

byte *Options::getBlinkCol() {                                                  //returns a pointer to the blinkColor array
    return blinkColor;
}

bool Options::getRainbow() {
    return rainbow;
}

int Options::getRainSpd() {
    return rainSpd;
}

bool Options::getBlink() {
    return blink;
}

byte Options::getBlinkSpd() {
    return blinkSpd;
}

int Options::getReadTime() {
    return readTime;
}

bool Options::getReadyBlink() {
    return readyBlink;
}

bool Options::getPrevTweet() {
    return onPrevious;
}

bool Options::getScroll() {
    return scroll;
}

bool Options::getSleep() {
    return sleep;
}

//==============================================================================

void Options::setBrightness(byte in) {
    brightness = in;
    inout.setBacklight(color[0], color[1], color[2], brightness);
}

void Options::setCol(byte r, byte g, byte b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    inout.setBacklight(color[0], color[1], color[2], brightness);
}

void Options::setBlinkCol(byte r, byte g, byte b) { 
    blinkColor[0] = r;
    blinkColor[1] = g;
    blinkColor[2] = b;
}

void Options::setRainbow(bool in) {
    rainbow = in;
}

void Options::setRainSpd(int in) {
    rainSpd = in;
}

void Options::setBlink(bool in) {
    blink = in;
}

void Options::setBlinkSpd(byte in) {
    blinkSpd = in;
}

void Options::setReadyBlink(bool in) {
    readyBlink = in;
}

void Options::setReadTime(int in) {
    readTime = in;
}

void Options::updateCol() {                                                     //force update the backlight color
    inout.setBacklight(color[0], color[1], color[2], brightness);
}

void Options::updateBlinkCol() {                                                //force update the tweet blink backlight color
    inout.setBacklight(blinkColor[0], blinkColor[1], blinkColor[2], brightness);
}

//==============================================================================

void Options::extractOption(String in) {                                        //used to extract the received option String from comms
    char type = in.charAt(0);                                                   //get the first char out of the input, this is the option type
    in = in.substring(1);                                                       //remove the the first character, it's no longer needed in there
    switch(type) {                                                              //check that char
        case 'b':                                                               //backlight brightness option
            getBrightnessVal(in);                                               //extract the necessary data, and apply the new setting
            break;
        case 'c':                                                               //backlight color option           
            getColorVal(in);                                                    //extract the necessary data, and apply the new setting   
            break;
        case 'd':                                                               //tweet blink options, contains color, speed, and enable    
            getTweetBlink(in);                                                  //extract the necessary data, and apply the new settings
            break;
        case 'e':                                                               //rainbow mode option, contains speed and enable
            getRainbow(in);                                                     //extract the necessary data, and apply the new settings
            break;
        case 'f':                                                               //read time option, contains read time value
            getReadTimeVal(in);                                                 //extract the necessary data, and apply the new settings
            break;
        case 'g':                                                               //previous/current tweet option, just a toggle
            getPrevTweet(in);                                                   //toggle the previous/current tweet
            break;
        case 'h':
            getScrollVal(in);
            break;
        case 's':
            getSleepVal(in);
        default:
            break;
    } 
}

void Options::getBrightnessVal(String in) {                                     //used to get the brightness value out of a transfer, and apply it
    String bright = in.substring(0, 3);                                         //get a substring containing the brightness value out
    setBrightness((byte)bright.toInt());                                        //set the brightness to that value converted to a byte
}

void Options::getColorVal(String in) {                                          //used to get the color value out of a transfer, and apply it
    //get substrings of each color value out
    String red = in.substring(0, 3);
    String green = in.substring(3, 6);
    String blue = in.substring(6, 9);
    setCol((byte)red.toInt(), (byte)green.toInt(), (byte)blue.toInt());         //convert each value to a byte, and send it to get applied
}

void Options::getTweetBlink(String in) {                                        //used to get the tweet blink values out of a transfer, and apply them  
    String enable = in.substring(0, 1);                                         //first get the enable setting out
    if(enable.toInt() == 0) {                                                   //check if the host enabled tweetblink, and then enable/disable 
        setBlink(false);
    }
    else {
        setBlink(true);
    }
    
    String spd = in.substring(1, 4);                                            //get a substring containing the blink speed value out
    setBlinkSpd((byte)spd.toInt());                                             //set the speed to that value converted to a byte
    
    //get the blink color values out in substrings
    String red = in.substring(4, 7);
    String green = in.substring(7, 10);
    String blue = in.substring(10, 13);
    setBlinkCol((byte)red.toInt(), (byte)green.toInt(), (byte)blue.toInt());    //convert each value to a byte, and send it to get applied
}

void Options::getRainbow(String in) {                                           //used to get the rainbow settings out of a transfer, and apply them
    String enable = in.substring(0, 1);                                         //first get the enable setting out
    if(enable.toInt() == 0) {                                                   //check if the host enabled rainbow mode, and then enable/disable 
        setRainbow(false);
    }
    else {
        setRainbow(true);
    }
    
    String spd = in.substring(1, 6);                                            //get a substring containing the rainbow speed value out
    setRainSpd(spd.toInt());                                                    //set the speed to that value converted to an int
}

void Options::getReadTimeVal(String in) {
    String time = in.substring(0, 5);                                           //get a substring containing the read time value out
    setReadTime(time.toInt());     
}

void Options::getPrevTweet(String in) {                                         //toggles the current/previous tweet
    String enable = in.substring(0, 1);                                         //get the enable setting out
    if(enable.toInt() == 0) {                                                   //if the previous tweet was disabled
        if(getPrevTweet()) {                                                      //only set it to the current tweet if we are on the previous one already
            //set the tweet to the current one
            onPrevious = false;
            lcd.printNewTweet(true);
        }
    }
    else {                                                                      //if the previous tweet was enabled
        if(twt.getPrevTweet() != "") {                                          //make sure there is a previous tweet first
            if(!getPrevTweet()) {                                               //only set it to the previous tweet if we are on the current one already
                //set the tweet to the previous one
                onPrevious = true;
                lcd.printNewTweet(false);
            }
        }
    }
}

void Options::getScrollVal(String in) {                                         //gets the scroll value out from the incoming data transfer
    String enable = in.substring(0, 1);                                         //get the enable setting out
    if(enable.toInt() == 0) {                                                   //scrolling was paused
        scroll = false;
        lcd.scrollNotification(true);                                           //tell the lcd to display the scrolling paused notification
    }
    else {                                                                      //scrolling was resumed
        scroll = true;
        lcd.scrollNotification(false);                                          //tell the lcd to display the scrolling paused notification
    }
}

void Options::getSleepVal(String in) {                                          //gets the sleep value out from the incoming data transfer
    String enable = in.substring(0, 1);                                         //get the enable setting out
    if(enable.toInt() == 0) {                                                   //sleep was disabled
        sleep = false;
    }
    else {                                                                      //sleep was enabled
        sleep = true;
    }
}