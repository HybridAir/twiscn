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
