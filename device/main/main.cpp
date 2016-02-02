#include <Arduino.h>
//#include <avr/io.h>
//#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */



#include "io.h"
#include "comms.h"





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
    init();             //arduino library init thing
    ioInit();
    commsInit();
    
    //connect
   
    

    
	while(1) {
        //monitor connection
        //monitor io
        monitorIo();

  //TwiscnUSB.println("Waiting for input...");
  // get input
  //get_input();
        
	}

	return 0;
}