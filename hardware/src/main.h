#ifndef MAIN_H_
#define MAIN_H_

#include "p24FJ16GA002.h"

//Number of lights in display
#define LIGHTCOUNT 16
//Size of receive buffer
#define RXBUFFSIZE 40
//Size of transmit buffer
#define TXBUFFSIZE 40
//Number of bytes in a frame
#define BYTECOUNT 6
//Newline character
#define NEWLINE 0x0A
//Size of our animation frame buffer
#define ANISIZE 200
//Bit on PORTA where the system light is located
#define SYSLIGHT 0x0004
//Bit on PORTA where the aux light is located
#define AUXLIGHT 0x0008
//Bit on PORTA where light 1 is located
#define LIGHT1 0x0001
//Bit on PORTA where light 2 is located
#define LIGHT2 0x0002

struct
{
	unsigned char data[BYTECOUNT];
	unsigned int dTime;
} typedef cFrame;


//Looks up the number of bytes for each message length (including start character)
unsigned int msgLen(unsigned char sChar);

//Checks if moving from the read position to an offset will cross the write
//position on the rx buffer.
unsigned int rxOffsetValid(unsigned int offset);

//Resets the animation
void aniReset();

//Pauses the animation
void aniPause();

//Start the animation
void aniStart();

//Gives the current receive buffer read index, and increments it. 
unsigned int nextRxRead();

//Load a frame buffer
void readFrame(unsigned char* buff);

//Set the frame buffer
void setFrame(unsigned char* buff);

//Transmit data over serial
void transData(unsigned char* buff, int len);

//Transmit a character over serial
void transChar(unsigned char data);

//Serial transmit interrupt
void _ISR _U1RXInterrupt(void);

//Serial recieve interrupt
void _ISR _U1RXInterrupt(void);

//Flash timer interrupt
void _ISR _T1Interrupt(void);

//Animation timer interrupt
void _ISR _T2Interrupt (void);

//System light timer interrupt
void _ISR _T3Interrupt (void);

//Notify of an error
void notify(unsigned char code);

//Processes the next message in the receive buffer
void processMsg();

#endif
