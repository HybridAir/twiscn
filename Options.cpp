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









//================================================================

void checkFN1() {
  boolean changed = debouncerFN1.update();
  if ( changed ) {
       // Get the update value
     int valueFN1 = debouncerFN1.read();
    if ( valueFN1 == HIGH) {  
       FN1buttonState = 0;
   
   } else {  
         FN1buttonState = 1;
         usbserial.println("FN1");
         FN1buttonPressTimeStamp = millis();
     
   }
  }
}

//================================================================

void checkFN2() {
  boolean changed = debouncerFN2.update();
  if ( changed ) {
       // Get the update value
     int valueFN2 = debouncerFN2.read();
    if ( valueFN2 == HIGH) {  
       FN2buttonState = 0;
   
   } else {  
         FN2buttonState = 1;
         usbserial.println("FN2");
         FN2buttonPressTimeStamp = millis();
     
   }
  }
}


#line 1 "options.ino"
void checkOption() {
  switch(finalStringBuffer[1]) {      //check the first char of the finalString array
    case 'b':      //backlight brightness option
      getBrightnessVal();
    case 'c':      //backlight color option
      getColorVal();   
      break;
    case 'd':      //new tweet blink option
      getTweetBlink();
      break;
    case 'e':      //rainbow mode option
      getRainbow();
      break;
  }  
}

//===============================================================

void getBrightnessVal() {
  char bright[4];
  bright[0] = finalStringBuffer[2];
  bright[1] = finalStringBuffer[3];
  bright[2] = finalStringBuffer[4];
  bright[3] = '\0';

  light = atoi(bright);
  brightness = light;
  setBacklight(r, g, b);
}

//===============================================================

void getColorVal() {
  char R[4];
  R[0] = finalStringBuffer[2];
  R[1] = finalStringBuffer[3];
  R[2] = finalStringBuffer[4];
  R[3] = '\0';
  
  char G[4];
  G[0] = finalStringBuffer[5];
  G[1] = finalStringBuffer[6];
  G[2] = finalStringBuffer[7];
  G[3] = '\0';
  
  char B[4];
  B[0] = finalStringBuffer[8];
  B[1] = finalStringBuffer[9];
  B[2] = finalStringBuffer[10];
  B[3] = '\0';

  r = atoi(R);
  g = atoi(G);
  b = atoi(B);

  brightness = light;
  setBacklight(r, g, b);
}

//===============================================================

void getTweetBlink() {
  if (finalStringBuffer[2] == '1') {
    tweetBlinkEnabled = 1;
  }
  else {
    tweetBlinkEnabled = 0;
  }
  
  char T[4];
  T[0] = finalStringBuffer[3];
  T[1] = finalStringBuffer[4];
  T[2] = finalStringBuffer[5];
  T[3] = '\0';  
  
  char R[4];
  R[0] = finalStringBuffer[6];
  R[1] = finalStringBuffer[7];
  R[2] = finalStringBuffer[8];
  R[3] = '\0';
  
  char G[4];
  G[0] = finalStringBuffer[9];
  G[1] = finalStringBuffer[10];
  G[2] = finalStringBuffer[11];
  G[3] = '\0';
  
  char B[4];
  B[0] = finalStringBuffer[12];
  B[1] = finalStringBuffer[13];
  B[2] = finalStringBuffer[14];
  B[3] = '\0';

  r2 = atoi(R);
  g2 = atoi(G);
  b2 = atoi(B);
  
  blinkTime = atoi(T);  
  timeToBlink= 0;
}

//===============================================================

void getRainbow() {
  if (finalStringBuffer[2] == '1') {
    rainbow = 1;
  }
  else {
    rainbow = 0;
    setBacklight(r, g, b);
  }  
  
  char T[4];
  T[0] = finalStringBuffer[3];
  T[1] = finalStringBuffer[4];
  T[2] = finalStringBuffer[5];
  T[3] = '\0';  
  
  howLongToWait = atoi(T);
}
