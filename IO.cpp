//handles basic device IO and sleep
#include <Arduino.h>
#include <avr/wdt.h>                                                            //needed to keep the whole system alive when USB is disconnected
#include "usbdrv.h"                                                             //the usbSofCount variable requires this
#include <Bounce.h>

IO::IO(Comms commsin) {                                                                      //default constructor, sets up all the inputs and outputs                                                
    pinMode(RESETPIN, OUTPUT);
    pinMode(CONLED, OUTPUT);
    pinMode(LCDPOWPIN, OUTPUT);
    pinMode(FN1PIN, INPUT);
    pinMode(FN2PIN, INPUT);
    dbFN1 = Bounce(); 
    dbFN2 = Bounce(); 
    dbFN1.attach(FN1PIN);
    dbFN2.attach(FN2PIN);
    comms = commsin;
}

void IO::connectionLED(byte mode) {                                             //controls the connection LED, needs a mode
    switch(mode) {
        case 0:                                                                 //turn the LED off
            digitalWrite(CONLED, LOW);
            break;  
        case 1:                                                                 //turn the LED on
            digitalWrite(CONLED, HIGH);
            break;
        case 2:                                                                 //blink the LED (non-blocking)
            currentMillis = millis();
            if(currentMillis - previousMillis > BLINKTIME) {
                previousMillis = currentMillis;
                if (blinkState) {
                    blinkState = 0;
                }
                else {
                    blinkState = 1;
                }
                connectionLED(blinkState);
            }
            break;
    }
}

void IO::checkButtons() {
    if(dbFN1.update()) {
        if(!dbFN1.read()) {                                                     //if the button was just released
            comms.sendBtn(1);                                                   //tell the host the user pushed FN1
        }                              
    }
    else if(dbFN2.update()) {
        if(!dbFN2.read()) {                                                     //if the button was just released
            comms.sendBtn(2);                                                   //tell the host the user pushed FN1
        }
    }
}





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


