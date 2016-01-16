#ifndef IO_H
#define	IO_H

#include <avr/io.h>
#include <util/delay.h>

#define CONN_LED            PC4

#define RED_LED             PB1
#define GREEN_LED           PD5
#define BLUE_LED            PD6
#define CONTRAST            PC2

void ioInit();


#endif