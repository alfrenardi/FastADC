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

/*
 * Different speed values can be chosen.
 * Lowest speed is 11 kHz, maximum speed is 90 kHz.
 * 
 * Speed affects actual resolution. Approximately:
 * 11 kHz => 10 bit
 * 23 kHz => 9 bit
 * 45 kHz => 8 bit 
 * 90 kHz => 7 bit
 */

void setup(){
    Serial.begin(9600);
}

void loop(){
    int i;
    FADC.start(A0,10); //Samples pin A0 with a resolution of 10 bit (at a speed 9 kHz)
    i = FADC.get();
    Serial.println(i, DEC);
    FADC.start(A3); //Samples another pin with the same speed and resolution as the last sample.
    i = FADC.get();
    Serial.println(i, DEC);
    FADC.start(A0,SPEED_11_kHz); //Resolution can be selected by speed
    i = FADC.get();
    Serial.println(i, DEC);
    FADC.stop(); //Stops sampling
    i = FADC.get(); //Invalid value is "-1"
    Serial.println(i, DEC);
}
