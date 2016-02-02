#include <Arduino.h>
//#include <avr/io.h>
//#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */



#include "io.h"
#include "comms.h"









int main(void) {
    
    init();             //arduino library init thing
    ioInit();
    commsInit();

    

    
	while(1) {
        //monitor connection
        monitorIo();

  //TwiscnUSB.println("Waiting for input...");
  // get input
  //get_input();
        
	}

	return 0;
}