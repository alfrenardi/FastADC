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
 * It is possible to calculate analog values directly in Volts.
 * It must be known the exact reference value.
 */

#include "FastADC.h"

void setup(){
    FADC.start(A0);
    Serial.begin(9600);
    double volts;
    analogReference(DEFAULT); //Default is Vcc, which is 5V on Arduino UNO board
    double volts = FADC.voltage(DEFAULT);
    Serial.println(volts, DEC); //sample in volts
    analogReference(INTERNAL);
    double volts = FADC.voltage(INTERNAL); //Internal is 1.1V on Arduino UNO board
    Serial.println(volts, DEC); //sample in volts
    analogReference(EXTERNAL); //AREF pin has to be connected to an external reference voltage that must be between Vcc and 0V
    double volts = FADC.voltage(3.5); //If an external reference has been choosen, it must be clarified the value used.
    Serial.println(volts, DEC); //sample in volts
}

void loop(){
}
