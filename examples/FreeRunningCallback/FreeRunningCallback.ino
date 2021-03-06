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

int volatile sum = 0;
int volatile samples = 0;
double average_value;

void user_function (int i) {
    /*
     * This function is made to be called as soon as a new sample is available.
     * In this way the board can do other tasks while is waiting for new samples.
     *
     * Warning: Tasks must be tiny or they can not be completed in the time between a sample and the other.
     */
     samples++;
     sum =+ i;
}

void setup(){
    FADC.start(A0);
    Serial.begin(9600);
    FADC.bind (*user_function); //Function "user_function" will be called from now
    delay(1000);
    FADC.unbind(); //Function "user_function" will be called no more
    average_value = sum / (double) samples;
    Serial.println (average_value, DEC);
}

void loop(){
}