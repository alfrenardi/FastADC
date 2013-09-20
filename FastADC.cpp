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
     * It puts the ADC value in the buffer and it calls the bounded function (if it have been defined)
     */
    if (running) {
        /*
         * FastADC must have been started.
         */
        //Truncates unuseful bits
        FADC.buffer = ADC >> (10 - resolution);;          
        FADC.available = true;
        FADC.bounded_function (FADC.buffer);
    }
}

// MEMBER FUNCTIONS

FastADC::FastADC(){
    //default values
    buffer = 0;
    pin_number = 0;
    available = false;
    running = false;
    resolution = 10;
    bounded_function = NULL;
}


void FastADC::start(uint pin, uint resolution_bits) { //TO DO: Check how it beahaves when pin is selected as output
    /*
     * This function starts the ADC. Resolution affects speed.
     *
     * Arg: Pin (between A0 and A5)
     * Arg: resolution (between 7 bit and 10).
     *
     * If arguments are not valid or missing, last used values are used.
     * If it is the first time, default values are used (pin = A0 and resolution = 10)
     */
    if(running){
        if (pin_numer == pin){
            /*
             * If the ADC is already running and the selected pin is the same,
             * the configuration is not modified
             * and the ADC keeps running.
             */
            return;
        } else {
            /*
             *  If not, ADC must be stopped and restarted with the new configuration.
             */
            this->stop();
            this->start(pin);
        }
    }
    else{
        /*
         * If not, the FastADC perform
         * some checks to input parameters and then
         * backups the old registers
         * configuration before starting
         */
        if(resolution_bits >= 7 and resolution_bits <= 10){
            /*
             * Resolution must be a valid number between 7 and 10
             */
            resolution = resolution_bits;
        }
        if(pin >= A0 and pin <= A5){
            /*
             * A valid ADC pin. 
             * ADC registers map pins 
             * with an offset of A0.
             * If the pin is not correct
             * the last used pin (es. A0)
             * will be selected.
             */
            pin_number = pin;
        }
        old_ADCSRB = ADCSRB;
        old_ADCSRA = ADCSRA;
        old_ADMUX = ADMUX;
        old_PRADC = PRR & PRADC;
    };
    // Internal registers are configured to start
    ADMUX = ((1<<REFS0)|(pin_number-A0)); // Pin number is affected by an offset
    ADCSRB = 0;
    ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | ((7-(10-resolution))<<ADPS0); //TO DO: Check on datasheet
    delay(100); // TO DO: Check
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

int FastADC::pin(){
    /*
     * Returns the pin which is being sampled.
     * If none has been selected, returns -1.
     * 
     */
    if(!running){
        // No pin
        return -1;
    };
    return pin;
}

void FastADC::bind(calback fun){ //TO DO: Check
    /*
     * It defines a custom callback. Function has to be void and it must take an integer as argument
     * If one has already been defined, the new one replace the old one.
     * 
     * Arg: A function
     * 
     */
    bounded_function = fun;
}

void FastADC::unbind(){
    /*
     * It removes custom callback.
     */ 
    bounded_function = NULL;
}


FastADC::~FastADC(void){
    ADCSRB = old_ADCSRB;
    ADCSRA = old_ADCSRA;
    ADMUX = old_ADMUX;
    PRR |= old_PRADC; 
}
