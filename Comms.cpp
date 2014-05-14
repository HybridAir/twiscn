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