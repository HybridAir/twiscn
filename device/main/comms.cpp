//jygfjhfuhfk

#include "comms.h"


void commsInit() {
    TwiscnUSB.begin();
    
    //do handshake in here
    //I guess just keep pinging the host until we get a response
    
}






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


void monitorConnection() {
    //check for any new data in the buffer
    //yeah
    
    
}
