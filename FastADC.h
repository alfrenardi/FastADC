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

#include <avr/interrupt.h>
extern "C" void __vector_21(void);

//Useful typedefs
typedef unsigned int uint;
typedef void (*callback)(int);

//Definitions that bind speed to significant bits
#define SPEED_9_kHz 10
#define SPEED_18_kHz 9
#define SPEED_37_kHz 8
#define SPEED_74_kHz 7

class FastADC {

private:

    int buffer;
    int pin_number;
    int resolution;

    bool available;
    bool running;

    char old_ADCSRB;
    char old_ADCSRA;
    char old_ADMUX;
    char old_PRADC;

    callback bounded_function;

    friend void __vector_21(void);

public:

    FastADC();

    void start(uint, uint);
    void stop();
    int get();
    int pin();

    void bind(callback);
    void unbind();

    ~FastADC();
    
};

extern FastADC FADC;
