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

//Definitions bind speed in kHz
#define SPEED_11_kHz 1
#define SPEED_23_kHz 2
#define SPEED_45_kHz 3
#define SPEED_90_kHz 4

class FastADC {

private:

    int volatile buffer;
    int pin_number;
    int speed_value;

    bool volatile available;
    bool volatile running;

    char old_ADCSRB;
    char old_ADCSRA;
    char old_ADMUX;
    char old_PRADC;

    callback bounded_function;

    friend void __vector_21(void);

    double reference;

public:

    FastADC();

    void start(uint, uint);
    void start(uint);
    void start();

    void stop();
    int get();
    int pin();

    double voltage();
    double voltage(int);
    double voltage(double);


    void bind(callback);
    void unbind();

    ~FastADC();
    
};

extern FastADC FADC;
