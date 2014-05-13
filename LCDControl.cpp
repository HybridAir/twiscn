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
