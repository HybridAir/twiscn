#ifndef __TwiscnUSB_h__
#define __TwiscnUSB_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "usbdrv.h"
#include "Print.h"

#define RING_BUFFER_SIZE 128

typedef uint8_t byte;


struct ring_buffer {
    unsigned char buffer[RING_BUFFER_SIZE];
    int head;
    int tail;
};


class TwiscnUSBDevice : public Print {
    private:
    ring_buffer *_rx_buffer;
    ring_buffer *_tx_buffer;

    public:
        TwiscnUSBDevice (ring_buffer *rx_buffer, ring_buffer *tx_buffer);
        void begin();
        void update();
        void refresh();
        void delay(long milliseconds);
        
        int available();
        int tx_remaining();
        int read();
        virtual size_t write(byte c);
        using Print::write;
};

extern TwiscnUSBDevice TwiscnUSB;

#endif
