#include "FastADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//======BUFFERS

static volatile char OnFastADC;
static volatile int analogValue;

//======INTERRUPT VECTOR

ISR(ADC_vect)
{
	analogValue=ADC;			//putting the analog value in the buffer
	OnFastADC = 2;				//new data available
}

//=======CONSTRUCTOR

FastADC::FastADC(){
	Old_ADCSRB = 0;
	Old_ADCSRA = 0;
	Old_ADMUX = 0;
	OnFastADC = 0;
}
	
//========INSTANCE METHODS

#include "FastADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//======BUFFERS

static volatile char OnFastADC;
static volatile int analogValue;

//======INTERRUPT VECTOR

ISR(ADC_vect)
{
	analogValue=ADC;			//putting the analog value in the buffer
	OnFastADC = 2;				//new data available
}

//=======CONSTRUCTOR

FastADC::FastADC(){
	Old_ADCSRB = 0;
	Old_ADCSRA = 0;
	Old_ADMUX = 0;
	OnFastADC = 0;
}
	
//========INSTANCE METHODS

void FastADC::StartADC(int pin=14,int res=10) { // default pin A0, 10 bit 9 KSamples for second
	if ((pin < 0) | (pin > 5)) {	//Only pin A0 to A5 can be readen
		if ((pin < 14) | (pin > 18)) {
			pin=1;
		} else {
			pin-=14;
		}
	}
	if ((res > 10)|(res<7)){
		res=10;
	}
	res -= 3;
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX
		OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
	}
	ADMUX = ((1<<REFS0)|(pin));
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | res); //free running mode, prescaler
	sei();
}

int FastADC::Get() {
	sei();
	while(OnFastADC==1);		//wait until new data are available
	cli();
	int i = analogValue;
	OnFastADC = 1;			//invalidate data for future readingsa
	sei();
	return i;				//return them
}

void FastADC::Stop() {	//restores old values
	if (OnFastADC) {
		ADCSRB = Old_ADCSRB;
		ADCSRA = Old_ADCSRA;
		ADMUX = Old_ADMUX;
		OnFastADC=0;
	}
}

//========PREINSTANTIATED OBJECTS

FastADC FreeRunningADC;