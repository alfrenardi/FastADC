#include "FastADC.h"

static int val;

void FastADC::StartADCSpeed(int pin=0) { //8 bit 38 KSamples for second
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
}

int FastADC::Get() {
	return val;
}

void FastADC::Stop() {	//restores old values
	if (OnFastADC) {
		ADCSRB = Old_ADCSRB;
		ADCSRA = Old_ADCSRA;
		ADMUX = Old_ADMUX;
	}
}

//void FastADC::GetArray(int* ext_val_num, int ext_num_val, int pin=0, char sel = 0) {
//	if (OnFastADC){
//		if (sel== 'R') {
//			Stop();
//			StartADCRes(pin, ext_num_val);
//		} else if (sel== 'S')
//			Stop();
//			StartADCRes(pin, ext_num_val);
//		}
//		for (int i = 0; i < ext_num_val; i++){
//			ext_val_num[i]=Get();
//		}
//	return;
//	}
//	if (sel== 'R') {
//		StartADCRes(pin, ext_num_val);
//	} else if (sel== 'S') {
//		Stop();
//		StartADCRes(pin, ext_num_val);
//	}
//	for (int i = 0; i < ext_num_val; i++) {
//		ext_val_num[i]=Get();
//	}
//	Stop();
//	return;
//}

ISR(ADC_vect)
{
	val=ADC;
}