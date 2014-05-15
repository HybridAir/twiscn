//handles device options/settings
#include <Arduino.h>

Options::Options() {                                                            //default constructor, sets up default options
    brightness = 255;                                                           //LCD brightness
    color = {0, 150, 255};                                                      //LCD Color
    blinkColor = {255, 0, 0};                                                   //color the LCD blinks when it gets a new tweet
    rainbow = false;                                                            //rainbow LCD mode
    rainSpd = 200;                                                              //rainbow color transition speed
    blink = false;                                                              //new tweet blink mode
    blinkSpd = 100;                                                             //tweet blink speed
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

//==============================================================================

void Options::setBrightness(byte in) {
    brightness = in;
}

void Options::setCol(byte r, byte g, byte b) {
    color = {r, g, b};
}

void Options::setBlinkCol(byte r, byte g, byte b) {                                  
    blinkColor = {r, g, b};
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

//==============================================================================

void Options::extractOption(String in) {                                        //used to extract the received option String from comms
    byte type = in.charAt(0);                                                   //get the first char out of the input, this is the option type
    in = in.substring(1);                                                       //remove the the first character, it's no longer needed in there
    switch(type) {                                                              //check that char
        case 'b':                                                               //backlight brightness option
            getBrightnessVal(in);                                                 //extract the necessary data, and apply the new setting
            break;
        case 'c':                                                               //backlight color option
            getColorVal(in);                                                        //extract the necessary data, and apply the new setting   
            break;
        case 'd':                                                               //tweet blink options, contains color, speed, and enable
            getTweetBlink(in);                                                      //extract the necessary data, and apply the new setting
            break;
        case 'e':                                                               //rainbow mode option, contains speed and enable
            getRainbow(in);
            break;
    } 
}

void Options::getBrightnessVal(String in) {                                     //used to get the brightness value out of a transfer, and apply it
    String bright = in.charAt(0, 2);                                            //get a substring containing the brightness value out
    setBrightness((byte)bright.toInt());                                        //set the brightness to that value converted to a byte
}

void Options::getColorVal(String in) {                                          //used to get the color value out of a transfer, and apply it
    //get substrings of each color value out
    String red = in.charAt(0, 2);
    String green = in.charAt(3, 5);
    String blue = in.charAt(6, 8);
    setCol((byte)red.toInt(), (byte)green.toInt(), (byte)blue.toInt());         //convert each value to a byte, and send it to get applied
}

void Options::getTweetBlink(String in) {                                        //used to get the tweet blink values out of a transfer, and apply them
    String enable = in.charAt(0);                                               //first get the enable setting out
    if(enable.toInt() == 0) {                                                   //"convert" the setting to a boolean, and apply it
        setBlink(false);
    }
    else {
        setBlink(true);
    }
    
    String spd = in.charAt(1, 3);                                               //get a substring containing the blink speed value out
    setBlinkSpd((byte)spd.toInt());                                             //set the speed to that value converted to a byte
    
    //get the blink color values out
    String red = in.charAt(4, 6);
    String green = in.charAt(7, 9);
    String blue = in.charAt(10, 12);
    setBlinkCol((byte)red.toInt(), (byte)green.toInt(), (byte)blue.toInt());    //convert each value to a byte, and send it to get applied
}

void Options::getRainbow(String in) {                                           //used to get the rainbow settings out of a transfer, and apply them
    String enable = in.charAt(0);                                               //first get the enable setting out
    if(enable.toInt() == 0) {                                                   //"convert" the setting to a boolean, and apply it
        setRainbow(false);
    }
    else {
        setRainbow(true);
    }
    
    String spd = in.charAt(1, 3);                                               //get a substring containing the blink speed value out
    setRainSpd(spd.toInt());                                                    //set the speed to that value converted to an int
}






void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
    r = map(r, 0, 255, 0, brightness);
    g = map(g, 0, 255, 0, brightness);
    b = map(b, 0, 255, 0, brightness);

    // common anode so invert!
    r = map(r, 0, 255, 255, 0);
    g = map(g, 0, 255, 255, 0);
    b = map(b, 0, 255, 255, 0);

    analogWrite(REDLITE, r);
    analogWrite(GREENLITE, g);
    analogWrite(BLUELITE, b);
}

void newTweetBlink() {
    if (tweetBlinkEnabled == 1) {
        if (timeToBlink == 1) {
            currentMillis5 = millis();
            if(currentMillis5 - previousMillis5 > blinkTime) {
                previousMillis5 = currentMillis5;
                if (blinkCount <= 4) {
                    if (blinkStatus == 0) {
                        brightness = 255;
                        setBacklight(r, g, b);
                        blinkStatus = 1;
                    }
                    else {
                        brightness = 255;
                        setBacklight(r2, g2, b2);
                        blinkStatus = 0;
                    }
                    if (blinkCount == 4) {
                        blinkCount = 0;
                        timeToBlink = 0;
                    }
                    else {
                        blinkCount++;
                    }
                }
            }
        }
        else if (timeToBlink == 0) {
            brightness = 255;
            setBacklight(r, g, b);      
        }
    }
}

void checkRainbow() {
    if (rainbow == 1) {   //do this if rainbow mode is on
        howLongItsBeen = millis() - lastTimeItHappened;
            if ( howLongItsBeen >= howLongToWait ) {   //if it's time, ycle through by one increment
      
        //Cycle through the rainbow
                if (color1 == 1) {   //do this if we're at the first rainbow section
                    if (rain < 255) {   //and only if we're below 255 increments already
                        setBacklight(rain, 0, 255-rain);
                        rain++;
                    }
                    else {   //if we're not in the first section, go to the next
                        color1 = 0;
                        color2 = 1;
                        rain = 0;   //reset the increment amount
                    }
                } 
                if (color2 == 1) {
                    if (rain < 255) {
                        setBacklight(255-rain, rain, 0);
                        rain++;
                    }
                    else {
                        color2 = 0;
                        color3 = 1;
                        rain = 0;
                    }
                } 
                if (color3 == 1) {
                    if (rain < 255) {
                        setBacklight(0, 255-rain, rain);
                        rain++;
                    }
                    else {
                        color3 = 0;
                        color1 = 1;
                        rain = 0;
                    }
                }  
   
                lastTimeItHappened = millis();   //save the last time we cycled through the rainbow by one
            }
        }
    }
