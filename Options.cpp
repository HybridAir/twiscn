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

void Options::extractOption(String in) {                                             //used to extract the received option String from comms
    //yeah
}