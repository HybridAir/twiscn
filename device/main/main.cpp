#include <Arduino.h>
//#include <avr/io.h>
//#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */

#include "usbdrv.h"


#include <LiquidCrystal.h>  
#include <TwiscnUSB.h>

#include "io.h"
#include "comms.h"

LiquidCrystal lcd(7, 8, 13, 10, 11, 12);



void get_input();


void get_input() {
  int lastRead;
  // when there are no characters to read, or the character isn't a newline
  while (true) { // loop forever
    if (TwiscnUSB.available()) {
      // something to read
      lastRead = TwiscnUSB.read();
      TwiscnUSB.write(lastRead);
      
      if (lastRead == '\n') {
        break; // when we get a newline, break out of loop
      }
    }
    
    // refresh the usb port for 10 milliseconds
    TwiscnUSB.delay(10);
  }
}



int main(void)
{
    init();
    ioInit();
    lcd.begin(16, 2);
   
    TwiscnUSB.begin();

    
    
    
	while(1)
	{
        //lcd.setCursor(0,0);
        //lcd.print(getButtons());
        
          // if (t == 0 || millis() > t+1000) {
        // usb.println("Hello World!");
        // t = millis();
        // }
        // usb.poll();
        
        
          // if(usb.available()) {
    // int size = usb.read(buffer);
    // if (size!=0) {
      // lcd.setCursor(0,0);
      // lcd.print(buffer[3]);
    // }
  // }
  // usb.poll();
  
    // print output
  TwiscnUSB.println("Waiting for input...");
  // get input
  get_input();
        
	}

	return 0;
}