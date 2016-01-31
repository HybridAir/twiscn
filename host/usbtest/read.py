import usb
import sys
sys.path.append("..")

from arduino.usbdevice import ArduinoUsbDevice


if __name__ == "__main__":
    try:
        theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x05df)

        print "Found: 0x%04x 0x%04x %s %s" % (theDevice.idVendor, 
                                              theDevice.idProduct,
                                              theDevice.productName,
                                              theDevice.manufacturer)
    except:
        pass




    import sys
    import time

    while 1 == 1:
        try:
            theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x05df)
            try:
                sys.stdout.write(chr(theDevice.read()))
                sys.stdout.flush()
            except:
                # TODO: Check for exception properly
                time.sleep(0.5)
                
        except:
            # TODO: Check for exception properly
            time.sleep(1)
            
       
        
