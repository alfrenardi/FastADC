#include "FastADC.h"

void FastADC::StartADCSpeed(int pin=0, int num_val_new =2) { //8 bit 38 KSamples for second
	num_val=num_val_new;
	if (pin > 5) {	//Only pin A0 to A5 can be readen
		pin=0;
	}
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX, DDRD
		char OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
		Old_DDRD = ADMUX;
		val_rd=0;
		val_wr=0;
		val_list = malloc (sizeof(int)*num_val);
	}
	ADMUX = (1<<REFS0)|(pin);
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS0)  |(1<<ADPS2)); //free running mode, prescaler = 32
}

void FastADC::StartADCRes(int pin=0, int num_val =2) { //10 bit 9 KSamples for second
	if (pin > 5) {	//Only pin A0 to A5 can be readen
		pin=0;
	}
	if (~OnFastADC) {  //Saves old values of ACSRB, ACSRA, ADMUX
		char OnFastADC=1;
		Old_ADCSRB = ADCSRB;
		Old_ADCSRA = ADCSRA;
		Old_ADMUX = ADMUX;
		val_rd=0;
		val_wr=0;
		val_list = malloc (sizeof(int)*num_val);
	}
	ADMUX = (1<<REFS0)|(pin);
	ADCSRB = 0;
	PRR &= ~(PRADC>>1); //ADC power reduction
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS0) | (1<<ADPS1) |(1<<ADPS2)); //free running mode, prescaler = 128
}


int FastADC::Get() {
	if (~OnFastADC) {
	 	return -1;
	}
	while (val_wr == val_rd);
	if (val_rd < (num_val-1)){
		return val_list [val_rd++];
	}
	val_rd=0;
	return val_list [num_val-1];
}

void FastADC::Stop() {	//restores old values
	if (OnFastADC) {
		ADCSRB = Old_ADCSRB;
		ADCSRA = Old_ADCSRA;
		ADMUX = Old_ADMUX;
		free(val_list);
	}
}

void GetArray(int* ext_val_num; int ext_num_val; int pin=0; char sel = 0) {
	if (OnFastADC){
		if (sel== 'R') {
			Stop();
			StartADCRes(pin, ext_num_val);
		} else if (sel== 'S')
			Stop();
			StartADCRes(pin, ext_num_val);
		}
		for (int i = 0; i < ext_num_val; i++){
			ext_val_num[i]=get();
		}
	return;
	}
	if (sel== 'R') {
		StartADCRes(pin, ext_num_val);
	} else if (sel== 'S')
		Stop();
		StartADCRes(pin, ext_num_val);
	}
	for (int i = 0; i < ext_num_val; i++){
		ext_val_num[i]=get();
	}
	Stop();
	return;
}
ISR(ADC_vect)
{
	val_list[val_wr++]=ADC;
	if (val_wr > (num_val-1)) {
		val_wr=0;
	}
}
