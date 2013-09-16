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

void setup(){
  	FADC.start(A0, 10);
  	Serial.begin(9600);
}

void loop(){
	int i = FADC.get();
	Serial.println(i, DEC);
}
