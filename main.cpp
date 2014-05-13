#include <Arduino.h>


//[INCLUDES]===============================================================
#include <HIDSerial.h>
#include <avr/wdt.h>         //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"          //the usbSofCount variable requires this
#include <Bounce.h>          //used for debouncing the FN buttons
#include <LiquidCrystal.h>
#include <PString.h>
#include <avr/pgmspace.h>



#define FN1PIN 4
#define SPEEDPIN A0
#define RESETPIN A1
#define FN2PIN A3
#define CONLED A4
#define LCD_COLS 16      //the amount of columns the display has
#define LCDPOWPIN 16      //pin used to control the contrast pot, and turn contrast on or off
#define MAX_INPUT 32

#define REDLITE 9    //used to be 3, which is now for usb
#define GREENLITE 5
#define BLUELITE 6

#define FADESPEED 10









//[LCD VARIABLES]===============================================================

void setup();
void loop();
void checkFN1();
void checkFN2();
void prepareLCD();
void CreateChar(byte code, PGM_P character);
void bootAnimation();
void connectingAnimation();
void connectedLCD();
void checkpot();
void displayBeginning(int textLength);
void displayUser();
void scrolltext();
void printtext();
void checkfrozen();
void setBacklight(uint8_t r, uint8_t g, uint8_t b);
void newTweetBlink();
void checkRainbow();
void checkcomms();
void checkmode();
void hostHandshake();
void spwm(int freq,int spin,int sp);
void stuffToloop();
void checkOption();
void getBrightnessVal();
void getColorVal();
void getTweetBlink();
void getRainbow();
void sleepDetect();
void sleepState();
void sleepWait();
#line 13
LiquidCrystal lcd(7, 8, 13, 10, 11, 12);
byte lcdStart = 0;      //stores the current position of the scrolling lcd text
byte lcdcount = 0;      //stores the Row2.length() offset
byte count = 0;      //stores the connection animation thing

//the following is used to store the boot logo characters into progmem
static prog_char PROGMEM top1[] = {0x1,0x1,0x3,0x3,0x7,0x7,0x3,0x1};
static prog_char PROGMEM top2[] = {0x10,0x10,0x18,0x18,0x1c,0x1c,0x18,0x10};
static prog_char PROGMEM left2[] = {0x8,0x1c,0x1e,0x1e,0x1e,0x18,0x0,0x0};
static prog_char PROGMEM left1[] = {0x0,0x0,0x0,0x1,0x3,0x7,0xf,0x18};
static prog_char PROGMEM right2[] = {0x2,0x7,0xf,0xf,0xf,0x3,0x0,0x0};
static prog_char PROGMEM right1[] = {0x0,0x0,0x0,0x10,0x18,0x1c,0x1e,0x3};

unsigned int finalSpeed = 0;   //converted speed value taken from the speed potentiometer
unsigned int interval2 = 3000;   //how long the lcd is kept frozen for
unsigned int freezeTime = 2000;
unsigned long previousMillis = 0;   //stores the last time the text was moved
unsigned long previousMillis2 = 0;   //the time the lcd was frozen
unsigned long previousMillis4 = 0;
unsigned long currentMillis4 = 0;
unsigned long currentMillis2 = 0;

boolean frozen = 0;   //stores if the lcd is currently frozen
boolean beginning = 0;   //stores if the text on the lcd is at the beginning
boolean waitforbegin = 0;   //stores if we are waiting for the beginning of the text
boolean unFroze = 0;

//[BACKLIGHT VARIABLES]===============================================================

byte mode = 1; //stores the backlight mode
byte brightness = 255;   //stores what brigtness the backlight should be at
byte light = 255;
byte blinkCount = 0;
byte r = 000;
byte g = 150;
byte b = 255;
byte r2 = 255;
byte g2 = 000;
byte b2 = 000;

boolean rainbow = 0;   //if rainbow mode is on or off
boolean tweetBlinkEnabled = 0;
boolean timeToBlink = 50;
boolean blinkStatus = 1;
unsigned long previousMillis5 = 0;
unsigned long currentMillis5 = 0;
unsigned int howLongToWait = 200;   //how long it takes for the rainbow to change a color level
unsigned long lastTimeItHappened = 0;   //the last time the color level was changed
unsigned int blinkTime = 50;
int howLongItsBeen;   //how long it has been since the color level was changed
int rain = 0;   //used for changing the color level
int color1 = 1;
int color2 = 0;
int color3 = 0;



//[COMMS VARIABLES]===============================================================

volatile uchar usbSofCount;
HIDSerial usbserial;

boolean newOutput = 0;   //stores if we just got new serial data
boolean conStatus = 0;
boolean newTweet = 0;
boolean greaterThan16 = 0;
boolean waitingFirst = 1;
boolean extendBuffer = 0;
boolean firstTweet = 1;
boolean sleepStatus = 0;
boolean asleep = 0;
boolean readyToSleep = 0;
boolean detectionState = 0;
boolean gotTweet = 0;
boolean gotUser = 0;
byte inByte;
byte row1Length = 0;
byte row2Length = 0;

char usbBuffer[32];
char inputStringBuffer[32];
char finalStringBuffer[160];
char Row1Array[16];
char Row2Array[160];

PString inputString(inputStringBuffer, sizeof(inputStringBuffer));
PString finalString(finalStringBuffer, sizeof(finalStringBuffer));
PString Row1(Row1Array, sizeof(Row1Array));
PString Row2(Row2Array, sizeof(Row2Array));

unsigned long currentstate;
unsigned long laststate;
unsigned long previousMillis6 = 0;
unsigned long previousMillis7 = 0;
unsigned long previousMillis8 = 0;
unsigned long currentMillis7 = 0;
unsigned long currentMillis8 = 0;
unsigned long lastTimeItHappened2 = 0;   //the last time the color level was changed
int howLongItsBeen2 = 0;   //how long it has been since the color level was changed
    
//[OTHER VARIABLES]===============================================================    
    
Bounce debouncerFN1 = Bounce(); 
Bounce debouncerFN2 = Bounce(); 

boolean runOnce = 0;
boolean FN1buttonState;
boolean FN2buttonState;
unsigned long FN1buttonPressTimeStamp;
unsigned long FN2buttonPressTimeStamp;   
    
//================================================================================

void setup() {
  digitalWrite(RESETPIN, HIGH);  
  pinMode(RESETPIN, OUTPUT);
  pinMode(CONLED, OUTPUT);
  pinMode(FN1PIN, INPUT);
  pinMode(FN2PIN, INPUT);
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT); 
  pinMode(LCDPOWPIN, OUTPUT);

  debouncerFN1.attach(FN1PIN);
  debouncerFN2.attach(FN2PIN);  
  
  PString inputString(inputStringBuffer, sizeof(inputStringBuffer));
  PString finalString(finalStringBuffer, sizeof(finalStringBuffer));
  
  prepareLCD();
  bootAnimation();  
  usbserial.begin();
  hostHandshake();
  
  /*uchar i;
  wdt_enable(WDTO_1S); // enable 1s watchdog timer
  usbInit();
  usbDeviceDisconnect(); // enforce re-enumeration
  for(i = 0; i<250; i++) { // wait 500 ms
    wdt_reset(); // keep the watchdog happy
    _delay_ms(2);
  }
  usbDeviceConnect();
  sei(); // Enable interrupts after re-enumeration */
}

//========================================================

void loop()
{
  usbPoll();
  //sleepWait();
  
  //if (readyToSleep == 1) {
    sleepDetect();
    sleepState();
  //}
  
  if (sleepStatus == 0) {

    newTweetBlink();
    checkRainbow();
    checkFN1();
    checkFN2();
    checkpot(); 
  
    checkcomms();
    checkmode(); 
    
    checkfrozen();   //see if the lcd is supposed to be frozen, and if it needs to unfreeze

    scrolltext();   
    if (firstTweet == 0) {
      if (waitforbegin == 0) {
        if (greaterThan16 == 1) {
          printtext();   //actually print the text
        }
      } 
    }  
  }
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
#line 1 "LCDtext.ino"
void prepareLCD() { 
  brightness = 0;
  digitalWrite(LCDPOWPIN, HIGH);
  lcd.begin(16,2);
}

//===============================================================
void CreateChar(byte code, PGM_P character) {
        byte* buffer = (byte*)malloc(8);
        memcpy_P(buffer, character,  8);
        lcd.createChar(code, buffer);
        free(buffer);
}

void bootAnimation() {  
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

//===============================================================

void connectingAnimation() {
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

//===============================================================

void connectedLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("latest data...");
}

//=======================================================================

void checkpot() {
  finalSpeed = analogRead(SPEEDPIN) / 2;   //get the value of the speed potentiometer, divided by 2
  //finalspeed = potvalue / 2;   //compute that value
}

//=======================================================================

void displayBeginning(int textLength) {   //used to show the beginning (first 16 characters) of the tweet
  lcd.setCursor(0, 1);
  lcd.print("                ");   //clear the bottom lcd row  
  for(int i = 1; i <= textLength; i++) {   //print each character in the TEXT array to the lcd
    lcd.setCursor(i - 1, 1);
    lcd.print(Row2Array[i]);
  }
}

//=======================================================================

void displayUser() {
    lcd.setCursor(0, 0);
    lcd.print("                ");   //clear the top lcd row  
    lcd.setCursor(0, 0);
    lcd.print(Row1);
}

//=================================================================

void scrolltext() {   //used to scroll the text on the bottom lcd row
  if (Row2.length() <= LCD_COLS) {   //do this if the tweet is less than 16 characters
    if (newTweet == 1) {   //check if we just got some new text
      lcd.clear();
      displayUser();
      displayBeginning(Row2.length() - 1);
      greaterThan16 = 0;
      newTweet = 0; 
      timeToBlink = 1;
    }  
  }
  else if (Row2.length() > LCD_COLS) {   //if tweet is longer than 16    
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

//===================================================================

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

//=======================================================================

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
#line 1 "backlight.ino"
//===============================================================

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

//===============================================================

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

//===============================================================

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
#line 1 "communications.ino"
void checkcomms() {   //used to process stuff in the serial buffer
  if (usbserial.available()) {   //do this if we got new data from the host
    usbserial.read((uint8_t*)usbBuffer);      //record the length of the data
    inByte = (uint8_t)usbBuffer[0];      //get the latest character out of the usbserial buffer 
    
    switch (inByte) {
      case '=':          //end of entire transfer    
        extendBuffer = 0;       
        newOutput = 1;   //Show that new data has been recieved and is ready to process
        break;                  
      case '+':      //end of current data packet
        extendBuffer = 1;      //open the next data buffer
        break;      
      default:      //add the new data to the current buffer as usual             
        if (extendBuffer == 1) {      //the transfer was longer than 32 bytes
          inputString.begin();
          inputString = usbBuffer;
          finalString += inputString;
        }
        else if (extendBuffer == 0) {      //the transfer was 32 bytes or less
          finalString.begin();
          finalString = usbBuffer;
        }
        break;
    }
  }
}

//======================================================================

void checkmode() {   //used to process text taken from the serial buffer
  if (newOutput > 0) {   //only do this if we just got stuff from checkComms (a '=' was sent)
    switch(finalStringBuffer[0]) {      //check the first char of the finalString array
      case '1':        //FN1 reply
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FN1");
        break;
      case '2':      //FN2 reply
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FN2");    
        break;
      case '@':      //start of a new tweet, first transfer contains username
        Row1.begin();
        Row1 += finalString;
        gotUser = 1;  //////////////////////////////////////////                               
        break;
      case '!':      //end of a new tweet, last transfer contains tweet text
        Row2.begin();
        Row2 += finalString;     
        gotTweet = 1; ////////////////////////////////
        //newTweet = 1;      //we have a new tweet ready to be displayed
        //lcdcount = Row2.length() - 15;   //let printtext() know how many characters to scroll this */
        break;
      case '$':      //option control
        checkOption();
        break;   
    }
    if (gotUser & gotTweet) {
      newTweet = 1;
      gotTweet = 0;
      gotUser = 0;
      lcdcount = Row2.length() - 15;
    }
    newOutput = 0;    
  }
}
#line 1 "handshake.ino"
void hostHandshake() {        //function used to let the TwitterScreen know that the host is ready
  while (conStatus == 0) {    //do this stuff while waiting for the host
  
    usbPoll();                //keep polling the USB port for any new data
    
    usbserial.println("`");   //spam this so the host will know that the twitterscreen is ready
    //digitalWrite(CONLED, HIGH);
    
    connectingAnimation();   
    for (int x=1;x<254;x++) {    //fade the connection LED on
      spwm(x,CONLED,FADESPEED);
    }
    connectingAnimation();
    for (int x=253;x>1;x--){     //fade the connection LED off
      spwm(x,CONLED,FADESPEED);
    }
  }
  
  digitalWrite(CONLED,HIGH);    //since we're connected by the time we get here, keep the connection LED on
  connectedLCD();  
}

//===============================================================

void spwm(int freq,int spin,int sp){        //function used to fade the connection LED
  stuffToloop();     //need to run some code in here since the parent function is blocking
  digitalWrite(spin,HIGH);                //code required for fake PWM
  delayMicroseconds(sp*freq);
  digitalWrite(spin,LOW);
  delayMicroseconds(sp*(255-freq));
}

//===============================================================

void stuffToloop() {
  usbPoll();                                //keep polling the USB port
  if (usbserial.available()) {              //check if we got any data from the host
    usbserial.read((uint8_t*)usbBuffer);                 //read that data into the HIDserial buffer
    unsigned char inByte = (uint8_t)usbBuffer[0];  //get the latest character out of the buffer
    if (inByte == '~') {                    //the ~ is the host acknowledging that it got the "`" from before
      conStatus = 1;                        //well I guess we're connected now
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
#line 1 "sleep.ino"
//======================================================================

void sleepDetect() {
    unsigned long currentMillis6 = millis();
  if(currentMillis6 - previousMillis6 > 500) {
    // save the last time you blinked the LED 
    previousMillis6 = currentMillis6; 
    if (detectionState == 0){
      laststate = usbSofCount; 
      detectionState = 1;
    }
    else if (detectionState == 1) {
    currentstate = usbSofCount;
    detectionState = 0;
      if (currentstate != laststate) {
        sleepStatus = 0;
      }
      else {
        sleepStatus = 1; 
      } 
    }
  }
}

//======================================================================

void sleepState() {
  if (sleepStatus == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Host is offline,");
    lcd.setCursor(0, 1);
    lcd.print("entering standby");
    boolean waitingToSleep = 1;
    previousMillis8 = millis();
    
    while(waitingToSleep == 1) {
      currentMillis8 = millis();
      if(currentMillis8 - previousMillis8 > 4000) {      
        waitingToSleep = 0;
        lcd.clear();
        brightness = 0;
        setBacklight(r, g, b);
        digitalWrite(LCDPOWPIN, LOW);
        conStatus = 0;
        asleep = 1;
      }
    }
  
    
    while(asleep == 1) {
      usbPoll();
      sleepDetect();
      if (sleepStatus == 0) {
        digitalWrite(CONLED,LOW);
        asleep = 0;
        runOnce = 0;
        readyToSleep = 0;
        digitalWrite(LCDPOWPIN, HIGH);
        bootAnimation();  
        hostHandshake();
        lcd.clear();
        newTweet = 1;
      }
    }
  }
}

//======================================================================

void sleepWait() {
  if (readyToSleep == 0) {
    howLongItsBeen2 = millis() - lastTimeItHappened2;
    if ( howLongItsBeen2 >= 5000 ) {
      readyToSleep = 1;
    }
  }
}

