/***********************************************************************************
 * Copyright © 2013 Daniele Faugiana, Alfredo Nardi
 *   

 * This file is part of "FastADC Arduino Library".
 * 
 * "FastADC Arduino Library" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * "FastADC Arduino Library" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with "WiPower Arduino Library". If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************************/


#include "FastADC.h"
#include "Arduino.h"
#include <avr/interrupt.h>

// GLOBAL OBJECT PRE-INSTANTIATION

FastADC FADC;

// ADC INTERRUPT VECTOR 

ISR(ADC_vect){
	/*
	 * When new data is available
	 * the interrupt handler is called.
	 * It puts the ADC value in the buffer.
	 */
	FADC.buffer = ADC;			
	FADC.available = true;
}

// MEMBER FUNCTIONS

FastADC::FastADC(){
	buffer = 0;
	pin_number = 0;
	available = false;
	running = false;
}


void FastADC::start(uint pin){ 
	/*
	 * This function starts the ADC at the 
	 * lowest speed.
	 */
	if(running){
		/*
		 * If the ADC is already running
		 * the configuration is not modified
		 * and the ADC keeps running.
		 */
		return;
	}
	else{
		/*
		 * If not, the FastADC perform
		 * some checks to input parameters and then
		 * backups the old registers
		 * configuration before starting
		 */
		if(pin >= A0 and pin <= A5){
			/*
			 * A valid ADC pin. 
			 * ADC registers map pins 
			 * with an offset of A0.
			 * If the pin is not correct
			 * the last used pin (es. A0)
			 * will be selected.
			 */
			pin_number = pin-A0;
		}
		old_ADCSRB = ADCSRB;
		old_ADCSRA = ADCSRA;
		old_ADMUX = ADMUX;
		old_PRADC = PRR & PRADC;
 	};
	// Internal registers are configured to start
	ADMUX = ((1<<REFS0)|pin_number);
	ADCSRB = 0;
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (PRESCALER<<ADPS0)); 
	delay(100);
	sei();
	running = true;

}


int FastADC::get(){
	if(!running){
		// FastADC needs to be started first
		return -1;
	};
	// Wait until new data are available
	while(!available);	
	cli();
	// Retrieve data from the buffer
	int i = buffer;
	available = false;			
	sei();
	return i;
}


void FastADC::stop(){	
	/*
	 * Does not erase the buffer
	 * but set back the old registers
	 * configuration for the ADC
	 */
	if(!running){
		return;
	}
	else{
		cli();
		ADCSRB = old_ADCSRB;
		ADCSRA = old_ADCSRA;
		ADMUX = old_ADMUX;
		PRR |= old_PRADC; 
		sei();
		running = false;
	}
}


FastADC::~FastADC(void){
	ADCSRB = old_ADCSRB;
	ADCSRA = old_ADCSRA;
	ADMUX = old_ADMUX;
	PRR |= old_PRADC; 
}
