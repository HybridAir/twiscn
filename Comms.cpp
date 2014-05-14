#include <Arduino.h>
#include <HIDSerial.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this

Comms::Comms() {                                                                //default constructor

}

void Comms::checkComms() {                                                      //checks if we got anything new from the host, and then processes it
    if (usb.available()) {                                                      //check if there's something in the usb buffer
        usb.read((uint8_t*)usbBuffer);                                          //put the data into a string (probably dont need the uint8_t*)
        int inByte = usbBuffer.charAt(0);                                       //first character is used to identify the data packet type
    
        switch (inByte) {                                                       //check what character it is, and process accordingly
            case '=':                                                           //marks the end of the entire transfer, must always be in its own packet
                //extendBuffer = 0;       
                //newOutput = true;                                               //show that new data has been received and is ready to process
                //don't need that either, just call the check right here
                checkType();                                                    //process the completed data transfer
                break; 
                //probably dont even need case + anymore, since we can easily add stuff to strings now
//            case '+':                                                           //signifies that this transfer will consist of an additional packet
//                //extendBuffer = 1;      //open the next data buffer
//                transferOut += usbBuffer;                                       //add the current packet to the output transfer String
//                break;      
            default:                                                            //this will only trigger if we got transfer type signifier (!, @, etc)            
//        if (extendBuffer == 1) {      //the transfer was longer than 32 bytes
                transferOut += usbBuffer;                                       //add the current packet to the output transfer String
//          inputString.begin();
//          inputString = usbBuffer;
//          finalString += inputString;
//        }
//        else if (extendBuffer == 0) {      //the transfer was 32 bytes or less
//          finalString.begin();
//          finalString = usbBuffer;
//        }
                break;
        }
    }
}

void Comms::checkType() {                                                       //used to check the type of transfer
  //if (newOutput > 0) {   //only do this if we just got stuff from checkComms (a '=' was sent)
    char type = transferOut.charAt(0);                                          //get the first char out of the transfer output, it's the transfer type
    transferOut = transferOut.substring(1);                                     //remove the the first character, it's no longer needed in there
    switch(type) {                                             //check the first char of the transferOut String
//        case '1':                                                               //FN1 reply
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("FN1");
//        break;
//      case '2':      //FN2 reply
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("FN2");    
//        break;
        case '@':                                                               //username transfer, also signifies the start of a new tweet
            userOut = transferOut;                                              //put the transfer into a new String for the username
            gotUser = true;                               
            break;
        case '!':                                                               //tweet transfer, ending for a new tweet     
            twtOut = transferOut;                                               //put the transfer into a new String for the tweet
            gotTweet = true;
            break;
        case '$':                                                               //option transfer
            checkOption();
            break;   
    }
    if (gotUser & gotTweet) {
        twt.setUser(twtOut);                                                    //these will go somewhere
        twt.setTweet(userOut);
        gotTweet = false;                                                       //already got the new tweet, so reset those vars
        gotUser = false;
      //lcdcount = Row2.length() - 15;
    }
    //newOutput = 0;    
}


void Comms::handshake() {                                                       //function used to establish a data connection with the host
    while (!connected) {                                                        //do this while we are not connected
        connectingAnimation();
        usbPoll();                                                              //keep polling the USB port for any new data
        usb.println("`");                                                       //continuously send this to the host so it knows the we are waiting for a handshake
        if (usb.available()) {                                                  //check if we got any data from the host
            usb.read((uint8_t*)usbBuffer);                                      //read that data into usbBuffer
            if (usbBuffer.charAt(0) == '~') {                                   //the ~ is the host acknowledging that it got the "`" from before
                connected = true;                                               //and now we are connected
            }
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

//void stuffToloop() {
//  usbPoll();                                //keep polling the USB port
//  if (usbserial.available()) {              //check if we got any data from the host
//    usbserial.read((uint8_t*)usbBuffer);                 //read that data into the HIDserial buffer
//    unsigned char inByte = (uint8_t)usbBuffer[0];  //get the latest character out of the buffer
//    if (inByte == '~') {                    //the ~ is the host acknowledging that it got the "`" from before
//      conStatus = 1;                        //well I guess we're connected now
//    }
//  }    
//}








//boolean newOutput = 0;   //stores if we just got new serial data
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