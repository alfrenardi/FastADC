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
    if (FADC.running) {
        /*
         * FastADC must have been started.
         */
        //Truncates unuseful bits
        FADC.buffer = ADC;          
        FADC.available = true;
        if (FADC.bounded_function) {
            FADC.bounded_function (FADC.buffer);
        }
    }
}

// MEMBER FUNCTIONS

FastADC::FastADC(){
    //default values
    buffer = 0;
    pin_number = A0;
    available = false;
    running = false;
    speed_value = 1; //Maximum resolution
    bounded_function = NULL;
    reference = 5; //5 Volts
}

void FastADC::start(uint pin) {
    /*
     * This is a definition used to have default arguments. Arduino enviroment doesn't support standard C/C++ default arguments syntax.
     */
    this->start(pin, 0);
}

void FastADC::start() {
    /*
     * This is a definition used to have default arguments. Arduino enviroment doesn't support standard C/C++ default arguments syntax.
     */
    this->start(0, 0);
}

void FastADC::start(uint pin, uint speed) {
    /*
     * This function starts the ADC. Resolution affects speed.
     *
     * Arg: Pin (between A0 and A5)
     * Arg: Speed (between 1 and 4).
     *
     * If arguments are not valid or missing, last used values are used.
     * If it is the first time, default values are used (pin = A0 and speed = 1)
     */
    if(running){
        if ((pin_number == pin) and (speed == speed_value)){
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
            this->start(pin, speed);
        }
    }
    else{
        /*
         * If not, the FastADC perform
         * some checks to input parameters and then
         * backups the old registers
         * configuration before starting
         */
        if(speed >= 1 and speed <= 4){
            /*
             * bits_number must be a valid number between 1 and 4
             */
            speed_value = speed;
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
    ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | ((8-speed_value)<<ADPS0));
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


double FastADC::voltage(){
    /*
     * Get the analog value in Volts.
     * It is necessary that the user explicit the voltage reference.
     */
    return this-> get()*reference/1023;
}

double FastADC::voltage(int ref){
    /*
     * Get the analog value in Volts.
     * It is necessary that the user explicit the voltage reference.
     *
     * Arg: reference as arduino-constants.
     */
    switch (ref){
        #ifdef DEFAULT
            case DEFAULT:
                reference = 5;
        #endif
        #ifdef INTERNAL //ATmega168, ATmega328
            case INTERNAL:
                reference = 1.1;
        #endif
        #ifdef INTERNAL1V1 //Arduino Mega only
            case INTERNAL1V1:
                reference = 1.1;
        #endif
        #ifdef INTERNAL2V5 //Arduino Mega only
            case INTERNAL2V5:
                reference = 2.56;
        #endif
    }
    return this-> get()*reference/1023;
}

double FastADC::voltage(double ref){
    /*
     * Get the analog value in Volts.
     * It is necessary that the user explicit the voltage reference.
     *
     * Arg: actual reference value in Volts.
     */
    reference = ref;
    return this-> get()*reference/1023;
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
    return pin_number;
}

void FastADC::bind(callback fun){
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