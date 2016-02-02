#ifndef IO_H
#define	IO_H

//#include <avr/io.h>
//#include <util/delay.h>
#include <Arduino.h>
#include <LiquidCrystal.h>  

#define CON_LED             PC4
#define SPEED_POT           PC0
#define FN1_BTN             PC3
#define FN2_BTN             PD4

#define RED_LED             PB1
#define GREEN_LED           PD5
#define BLUE_LED            PD6
#define CONTRAST            PC2

void ioInit();
void monitorIo();
void setConLed(bool enabled);
void setBacklight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
void setContrast(bool enabled);

uint16_t getSpeed();
uint8_t getButtons();


#endif