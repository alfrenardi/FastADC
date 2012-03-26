#include "FastADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//======BUFFERS

static volatile int analogValue;
//static volatile int newData;

//======INTERRUPT VECTOR

ISR(ADC_vect)
{
//	volatile int sreg = SREG;	//saving the status register
	analogValue=ADC;			//putting the analog value in the buffer
	OnFastADC = 2;				//new data available
//	SREG = sreg;				//restoring the status register
}

//=======CONSTRUCTOR

FastADC::FastADC(){
	Old_ADCSRB = 0;
	Old_ADCSRA = 0;
	Old_ADMUX = 0;
	OnFastADC = 0;
}
	
//========INSTANCE METHODS

void FastADC::StartADCMaxSpeed(int pin=0) { //7 bit 74 KSamples for second
	if (pin > 5) {	//Only pin A0 to A5 can be readen
		pin=0;
	}
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX
		char OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
	}
	ADMUX = (1<<REFS0)|(pin);
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS1); //free running mode, prescaler = 16
	sei();
}

void FastADC::StartADCSpeed(int pin=0) { //8 bit 37 KSamples for second
	if (pin > 5) {	//Only pin A0 to A5 can be readen
		pin=0;
	}
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX, DDRD
		char OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
	}
	ADMUX = (1<<REFS0)|(pin);
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS0)  |(1<<ADPS2)); //free running mode, prescaler = 32
	sei();
}

void FastADC::StartADCRes(int pin=0) { //10 bit 9 KSamples for second
	if (pin > 5) {	//Only pin A0 to A5 can be readen
		pin=0;
	}
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX
		char OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
	}
	ADMUX = (1<<REFS0)|(pin);
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS0) | (1<<ADPS1) |(1<<ADPS2)); //free running mode, prescaler = 128
	sei();
}

int FastADC::Get() {
	sei();
	while(OnFastADC==1);		//wait until new data are available
//	cli();
	int i = analogValue;
	OnFastADC = 1;			//invalidate data for future readingsa
//	sei();
	return i;				//return them
}

void FastADC::Stop() {	//restores old values
	if (OnFastADC) {
		ADCSRB = Old_ADCSRB;
		ADCSRA = Old_ADCSRA;
		ADMUX = Old_ADMUX;
	}
}

//========PREINSTANTIATED OBJECTS

FastADC FreeRunningADC;