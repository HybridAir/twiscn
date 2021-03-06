#include <Arduino.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "oddebug.h"

#include "TwiscnUSB.h"


ring_buffer rx_buffer = { { 0 }, 0, 0 };
ring_buffer tx_buffer = { { 0 }, 0, 0 };


inline int store_char(unsigned char c, ring_buffer *the_buffer) {
    int i = (the_buffer->head + 1) % RING_BUFFER_SIZE;

    if (i != the_buffer->tail) {
        the_buffer->buffer[the_buffer->head] = c;
        the_buffer->head = i;
        return 1;
    }
    
    return 0;
}


TwiscnUSBDevice::TwiscnUSBDevice(ring_buffer *rx_buffer, ring_buffer *tx_buffer) {
    _rx_buffer = rx_buffer;
    _tx_buffer = tx_buffer;
}


void TwiscnUSBDevice::begin() {
    cli();
    usbInit();   
    usbDeviceDisconnect();
    
    uchar   i;
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(10);
    }
    
    usbDeviceConnect();
    sei();
}
    

void TwiscnUSBDevice::update() {
    refresh();
}


void TwiscnUSBDevice::refresh() {
    usbPoll();
}


void TwiscnUSBDevice::delay(long milli) {
    unsigned long last = millis();
    while (milli > 0) {
        unsigned long now = millis();
        milli -= now - last;
        last = now;
        refresh();
    }
}

int TwiscnUSBDevice::available() {
    return (RING_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % RING_BUFFER_SIZE;
}

int TwiscnUSBDevice::tx_remaining() {
    return RING_BUFFER_SIZE - (RING_BUFFER_SIZE + _tx_buffer->head - _tx_buffer->tail) % RING_BUFFER_SIZE;
}

 
int TwiscnUSBDevice::read() {
    if (_rx_buffer->head == _rx_buffer->tail) {
        return -1;
    } 
    else {
        unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
        _rx_buffer->tail = (_rx_buffer->tail + 1) % RING_BUFFER_SIZE;
        return c;
    }
}


size_t TwiscnUSBDevice::write(byte c) {
    return store_char(c, _tx_buffer);
}


int tx_available() {
    return (RING_BUFFER_SIZE + tx_buffer.head - tx_buffer.tail) % RING_BUFFER_SIZE;
}


int tx_read() {
    if (tx_buffer.head == tx_buffer.tail) {
        return -1;
    } 
    else {
        unsigned char c = tx_buffer.buffer[tx_buffer.tail];
        tx_buffer.tail = (tx_buffer.tail + 1) % RING_BUFFER_SIZE;
        return c;
    } 
}




/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"{
#endif 
PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* Since we define only one feature report, we don't use report-IDs (which
 * would be the first byte of the report). The entire report consists of 1
 * opaque data bytes.
 */

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  usbRequest_t    *rq = (usbRequest_t*)((void *)data);

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
	    static uchar dataBuffer[1];  /* buffer must stay valid when usbFunctionSetup returns */
	    if (tx_available()) {
	      dataBuffer[0] = tx_read();
	      usbMsgPtr = dataBuffer; /* tell the driver which data to return */
	      return 1; /* tell the driver to send 1 byte */
	    } else {
	      // Drop through to return 0 (which will stall the request?)
	    }
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */

	  // TODO: Check race issues?
	  store_char(rq->wIndex.bytes[0], &rx_buffer);

        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}
#ifdef __cplusplus
} // extern "C"
#endif

TwiscnUSBDevice TwiscnUSB = TwiscnUSBDevice(&rx_buffer, &tx_buffer);