#include <Arduino.h>
#include <HIDSerial.h>
#include <avr/wdt.h>         //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"          //the usbSofCount variable requires this
#include <Bounce.h>          //used for debouncing the FN buttons
#include <LiquidCrystal.h>
#include <PString.h>
#include <avr/pgmspace.h>


//[INCLUDES]===============================================================




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




//=======================================================================

void checkpot() {
  finalSpeed = analogRead(SPEEDPIN) / 2;   //get the value of the speed potentiometer, divided by 2
  //finalspeed = potvalue / 2;   //compute that value
}






