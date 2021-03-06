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

/*
 * Free running mode is a way to use analog to digital converters at high rate.
 * 
 * Arduino can use this tecnique on one pin per time at different speeds.
 */

#include "FastADC.h"

void setup(){
    FADC.start(A0);
    Serial.begin(9600);
}

void loop(){
    int i = FADC.get();
    Serial.println(i, DEC); //sample valued from 0 to 1023
    double volts = FADC.voltage();
    Serial.println(volts, DEC); //sample in volts
}
