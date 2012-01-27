#include "p24FJ16GA002.h"
#include "main.h"

_CONFIG2( 	IESO_ON & 
			OSCIOFNC_ON &
			SOSCSEL_SOSC &
			WUTSEL_LEG &
			FNOSC_FRCDIV &
			FCKSM_CSDCMD &
			OSCIOFNC_ON &
			I2C1SEL_PRI &
			POSCMOD_NONE )

_CONFIG1(	JTAGEN_OFF &
			GCP_OFF &
			GWRP_OFF &
			ICS_PGx1 &
			FWDTEN_OFF &
			WINDIS_OFF &
			FWPSA_PR128 &
			WDTPS_PS32768 )

//Current frame we are displaying
unsigned char curFrame[BYTECOUNT];
//The light intensities for each pixel
volatile unsigned char lightInt[LIGHTCOUNT];
//Count for the intensities of lights
unsigned int lCount;

//Status of the System Light
unsigned char sysLight;
//Status of the Aux. Light
unsigned char auxLight;
//Current error code
unsigned char sysErrorCode;
//Error code display counter
unsigned char errDisCount;

//Frames for our animation
cFrame aniFrames[ANISIZE];
//The current position we are in our animation
volatile unsigned int aniPos;
//Number of times we repeat the animation (0 if infinite)
unsigned int aniRepeat;
//Number of times we have repeated the current animation
volatile unsigned int aniRepeatCount;
//The number of 'ticks' we have been on the current frame
volatile unsigned int aniFrameTicker;
//The period of each frame
unsigned int aniTickPeriod;
//If our animation is playing or not
volatile unsigned char aniEnabled;

//Buffer we receive in
volatile unsigned char rxBuff[RXBUFFSIZE];
//Position for writing in the receive buffer
volatile unsigned char rxWritePos;
//Position for reading in the receive buffer
unsigned char rxReadPos;

//Buffer we transmit from
unsigned char txBuff[TXBUFFSIZE];
//Position for reading in the transmit buffer
volatile unsigned char txReadPos;
//Position for writing in the transmit buffer
unsigned char txWritePos;


//Looks up the number of bytes for each message length (including start character)
unsigned int msgLen(unsigned char sChar)
{
	int out;
	switch(sChar)
	{
		case 'C': //Set current frame
			out = 7;
			break;
		case 'B': //Get the current frame
			out = 1;
			break;
		case 'X': //Set the animation position
			out = 3;
			break;
		case 'Y': //Get the animation position
			out = 1;
			break;
		case 'D': //Write an animation frame
			out = 9;
			break;
		case 'S': //Start the animation
			out = 2;
			break;
		case 'P': //Pause the animation
			out = 1;
			break;
		case 'R': //Reset the animation
			out = 1;
			break;
		case 'T': //Set the frame period
			out = 3;
			break;
		case 'L': //Turn the aux light on or off
			out = 2;
			break;
		default: //Bad start character
			out = 0;
			break;
	}
	
	return out;
}

//Checks if moving from the read position to an offset will cross the write
//position on the rx buffer.
unsigned int rxOffsetValid(unsigned int offset)
{
	unsigned int tempWrite = rxWritePos;
	if(tempWrite < rxReadPos) //It's wrapped
		tempWrite += RXBUFFSIZE;
	
	if(rxReadPos + offset < tempWrite)
		return 1;
	else
		return 0;
}	


//Resets the animation
void aniReset()
{
	aniPause();
	TMR2 = 0x00;
	aniPos = 0;
	aniFrameTicker = 0;
	int i;
	for(i = 0; i<LIGHTCOUNT; ++i)
	{
		lightInt[i] = 0;
	}
}

//Pauses the animation
void aniPause()
{
	_T2IE = 0; //Enable interrupt
	T2CONbits.TON = 0; //Turn on timer
	aniEnabled = 0;
}

//Start the animation
void aniStart()
{
	_T2IE = 1; //Enable interrupt
	T2CONbits.TON = 1; //Turn on timer
	aniEnabled = 1;
	setFrame(aniFrames[aniPos].data);
}

//Gives the current receive buffer read index, and increments it. 
unsigned int nextRxRead()
{
	int temp = rxReadPos;
	rxReadPos++;
	rxReadPos %= RXBUFFSIZE;
	return temp;
}

//Load a frame buffer
void readFrame(unsigned char* buff)
{
	int i;
	for (i = 0; i<BYTECOUNT; ++i)
	{
		buff[i] = rxBuff[nextRxRead()];
	}
}

//Set the frame buffer
void setFrame(unsigned char* buff)
{
	int i;
	int lightnum = 0;
	int bitcount = 0;
	
	unsigned char curLight = 0;
	for(i = 0; i<BYTECOUNT; ++i) //Loop through bytes
	{
		int j;
		for(j = 0; j<8; ++j) //Loop through bits
		{
			curLight <<= 1; //Shift one byte
			curLight += (buff[i] >> (7 - j)) & 0x01;
			bitcount++;
			if (bitcount == 3)
			{
				lightInt[lightnum] = curLight;
				bitcount = 0;
				lightnum++;
				curLight = 0;
			}
		}
	}
}

//Transmit data over serial
void transData(unsigned char* buff, int len)
{
	int i;
	for(i = 0; i<len; ++i)
	{
		txBuff[txWritePos] = buff[i];
		++txWritePos;
		txWritePos %= TXBUFFSIZE;
	}
	
	while(!_UTXBF && txReadPos != txWritePos)
	{
		U1TXREG = txBuff[txReadPos];
		++txReadPos;
		txReadPos %= TXBUFFSIZE;
	}
}

//Transmit a character over serial
void transChar(unsigned char data)
{
	transData(&data,1);
}


//Serial transmit interrupt
void __attribute__((interrupt, no_auto_psv)) _ISR _U1TXInterrupt (void)
{
	if(_U1TXIF)
	{
		while(!_UTXBF && txReadPos != txWritePos) //Check for stuff in the transfer buffer
		{
			U1TXREG = txBuff[txReadPos];
			++txReadPos;
			txReadPos %= TXBUFFSIZE;
		}
		
		_U1TXIF = 0;
	}
}

//Serial recieve interrupt
void __attribute__((interrupt, no_auto_psv)) _ISR _U1RXInterrupt(void)
{
	if(_U1RXIF)
	{
		while(_URXDA)
		{
			unsigned char in = U1RXREG;
			
			rxBuff[rxWritePos] = in;
			++rxWritePos;
			rxWritePos %= RXBUFFSIZE;
		}	
		
		_U1RXIF = 0;
	}
}


//Flash timer interrupt
void __attribute__((interrupt, no_auto_psv)) _ISR _T1Interrupt (void)
{
	lCount++;
	lCount %= 49;
	
	unsigned int newVal = 0;
	int i;
	for(i = 0; i<LIGHTCOUNT; ++i)
	{
		newVal <<= 1;
		if (lightInt[i]*lightInt[i] > lCount)
		{
			newVal += 1;
		}
	}
	
	PORTB = newVal;
	
	unsigned int aVal = (~(LIGHT1 | LIGHT2)) & PORTA; //Set up the last two lights
	if (newVal & 0x0001)
		aVal |= LIGHT1;
	if (newVal & 0x0002)
		aVal |= LIGHT2;
	PORTA = aVal;
	
	_T1IF = 0;
} // _T1Interrupt


//Animation timer interrupt
void __attribute__((interrupt, no_auto_psv)) _ISR _T2Interrupt (void)
{
	++aniFrameTicker;
	if(aniFrameTicker >= aniFrames[aniPos].dTime) //New frame
	{
		++aniPos;
		if(aniFrames[aniPos].dTime == 0) //Redirect to new frame
		{
			++aniRepeatCount;
			if(aniRepeatCount != aniRepeat || aniRepeat == 0) //Check if we are done repeating
			{
				unsigned int temp = aniFrames[aniPos].data[0] << 8; //Get the new position
				temp += aniFrames[aniPos].data[1];
				aniPos = temp;
			}
			else //We are done looping through the animation
			{
				aniPause();
				int i;
				for(i = 0; i<LIGHTCOUNT; ++i)
				{
					lightInt[i] = 0;
				}
			}	
		}
		
		if(aniEnabled)
		{
			aniFrameTicker = 0;
			setFrame(aniFrames[aniPos].data);
		}
	}
	
	_T2IF = 0;
} // _T2Interrupt


//System light timer interrupt
void __attribute__((interrupt, no_auto_psv)) _ISR _T3Interrupt (void)
{
	unsigned int tPort = PORTA;

	// Add a pause after blinking the error
	if (errDisCount == 0)
	{
		errDisCount = sysErrorCode*2;
	}
	else
	{
		sysLight = ~sysLight;
		errDisCount--;
	}

	//Turn on/off the system light
	if (sysLight)
	{
		tPort |= SYSLIGHT;
	}
	else
	{
		tPort &= ~SYSLIGHT;
	}

	PORTA = tPort;

	_T3IF = 0;
} // _T3Interrupt



//Processes the next message in the receive buffer
void processMsg()
{
	unsigned int i;
	unsigned int temp;
		
	switch(rxBuff[nextRxRead()])
	{
		case 'C': //Set current frame
			if(aniEnabled)
				aniPause();
			readFrame(curFrame);
			setFrame(curFrame);
			sysErrorCode = 0;
			break;
		case 'B': //Get the current frame
			transChar('B');
			transData(curFrame, BYTECOUNT);
			transChar(0x0A);
			sysErrorCode = 0;
			break;
		case 'X': //Set the animation position
			temp = (rxBuff[nextRxRead()] << 8);
			temp += rxBuff[nextRxRead()];
			aniPos = temp;
			sysErrorCode = 0;
			break;
		case 'Y': //Get the animation position
			transChar('Y');
			transChar(aniPos >> 8);
			transChar(aniPos & 0x00FF);
			transChar(0x0A);
			sysErrorCode = 0;
			break;
		case 'D': //Write an animation frame
			temp = (rxBuff[nextRxRead()] << 8);
			temp += rxBuff[nextRxRead()];
			aniFrames[aniPos].dTime = temp;
			for(i = 0; i<BYTECOUNT; ++i)
			{
				aniFrames[aniPos].data[i] = rxBuff[nextRxRead()];
			}
			aniPos++;
			sysErrorCode = 0;
			break;
		case 'S': //Start the animation
			aniRepeatCount = 0;
			aniRepeat = rxBuff[nextRxRead()];
			aniStart();
			sysErrorCode = 0;
			break;
		case 'P': //Pause the animation
			aniPause();
			sysErrorCode = 0;
			break;
		case 'R': //Reset the animation
			aniReset();
			sysErrorCode = 0;
			break;
		case 'T': //Set the frame period
			temp = (rxBuff[nextRxRead()] << 8);
			temp += rxBuff[nextRxRead()];
			PR2 = temp;
			TMR2 = 0x00;
			aniTickPeriod = temp;
			sysErrorCode = 0;
			break;
		case 'L': //Turn the aux light on or off
			temp = PORTA;
			if(rxBuff[nextRxRead()])
			{
				temp |= AUXLIGHT;
			}
			else
			{
				temp &= ~AUXLIGHT;
			}
			PORTA = temp;
			sysErrorCode = 0;
			break;
		default: //Invalid command
			sysErrorCode = 1;
			break;
	}
	
	++rxReadPos; //Pass the newline at the end
}	

int main() 
{
	_RCDIV = 0;		//Set clock to 8MHz

	// RB1 RX
	// RB0 TX
	TRISA = 0x0000;
	TRISB = 0x0002;
	PORTA = 0x0000;
	PORTB = 0x0000;

	AD1PCFG = 0xFFFF;			//Disable all analog channels

	_RP0R = 3;      //Assign TX to RB0
	_U1RXR = 1;	    //Assign RX to RB1
	
	//Setup Serial Comm
	_UARTEN = 0;	//Disable UART
	_USIDL = 0;		//Continue operation in idle mode
	_IREN = 0;		//Disable InfraRed communication
	_RTSMD = 1;		//Simplex Mode
	_UEN = 0;		//RX and TX pins used, flow control not
	_WAKE = 0;		//No wakeup from sleep mode
	_LPBACK = 0;	//Disable loopback
	_ABAUD = 0;		//Disable auto baud rate detect
	_RXINV = 0;		//RX idle level is 1
	_BRGH = 0;		//Low speed
	_PDSEL = 0;		//8 bit, no parity
	_STSEL = 0;		//1 stop bit

	U1BRG = 25;		//Set baud rate to 9600

	_UTXISEL0 = 0;	//Interrupt when space is open in the TX FIFO
	_UTXISEL1 = 0;
	_UTXINV = 0;	//TX idle level is 1
	_UTXBRK = 0;	//Disable sync
	_UTXEN = 0;		//Disable transmission
	_URXISEL1 = 0;	//Interrupt when unsigned character is received
	_URXISEL0 = 0;
	_ADDEN = 0;		//Address mode disabled
	_U1TXIE = 1;	//Enable transmit interrupt
	
	_U1RXIF = 0;	//Clear receive flag
	_U1RXIP = 7;	//Set receive interrupt to highest priority
	_U1RXIE = 1;	//Enable receive interrupt

	_U1TXIP = 6;	//Set transmit interrupt to higher priority
	
	_UARTEN = 1;	//Enable UART
	_UTXEN = 1;		//Enable transmission
	
	
	//Flash timer
	T1CON = 0x00;
	TMR1 = 0x00;
	PR1 = 0x0010; //Timer counts
	_T1IP = 0x01;
	T1CONbits.TCKPS1 = 1; //Set timer prescaler
	T1CONbits.TCKPS0 = 0;
	_T1IF = 0; //Clear timer interrupt flagC
	_T1IE = 1; //Enable interrupt
	T1CONbits.TON = 1; //Turn on timer
	
	//Frame timer
	T2CON = 0x00;
	TMR2 = 0x00;
	PR2 = 0x4000; //Timer counts
	_T2IP = 0x01;
	T2CONbits.TCKPS1 = 1; //Set timer prescaler
	T2CONbits.TCKPS0 = 1;
	_T2IF = 0; //Clear timer interrupt flag
	_T2IE = 0; //Enable interrupt
	T2CONbits.TON = 0; //Turn on timer
	
	//Sys light timer
	T3CON = 0x00;
	TMR3 = 0x0000;
	PR3 = 0x1000; //Timer counts
	_T3IP = 0x01;
	T3CONbits.TCKPS1 = 1; //Set timer prescaler
	T3CONbits.TCKPS0 = 1;
	_T3IF = 0; //Clear timer interrupt flag
	_T3IE = 1; //Enable interrupt
	T3CONbits.TON = 1; //Turn on timer
	
	//Animation stuff
	aniPos = 0;
	aniRepeat = 0;
	aniRepeatCount = 0;
	aniFrameTicker = 0;
	aniTickPeriod = 0x4000;
	aniEnabled = 0;
	
	//Data setup
	rxWritePos = 0;
	rxReadPos = 0;
	txWritePos = 0;
	txReadPos = 0;
	
	lCount = 0;
	int i;
	for(i = 0; i<16; ++i)
	{
		lightInt[i] = 0;
	}

	//Notification setup
	auxLight = 0;
	sysLight = 0;
	sysErrorCode = 2;
	errDisCount = 0;

	//Main processig loop
	while(1)
	{
		if (rxReadPos != rxWritePos)
		{
			unsigned char startC = rxBuff[rxReadPos];
			unsigned int temp = msgLen(startC);
			
			if(temp == 0) //Bad start character
			{
				nextRxRead(); //Skip to next character
			}
			else if(rxOffsetValid(temp)) //There is enugh room to read the message
			{
				//Make sure there is a newline at the end of the message
				if(rxBuff[(rxReadPos + temp) % RXBUFFSIZE] == NEWLINE)
				{
					processMsg();
					if(_T3IE) //Disable the system light
					{
						_T3IE = 0;
						T3CONbits.TON = 0;
						//PORTA |= SYSLIGHT;
					}
				}
				else //No newline, bad message
				{
					nextRxRead(); //Skip to next character
				}
			}
		}
	}

	return 0;
}
