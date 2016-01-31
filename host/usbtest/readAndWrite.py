#!/usr/bin/python

import usb
import sys
import time
sys.path.append("..")

from arduino.usbdevice import ArduinoUsbDevice


if __name__ == "__main__":
    try:
        theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x05df)
    except:
        sys.exit("No DigiUSB Device Found")

    while True:
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