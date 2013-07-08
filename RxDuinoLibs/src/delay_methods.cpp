/************************************************************************
 * RXDUINO Software, All Rights Reserved 2012
 * This program is not free software; you can not redistribute it and/or modify
 * it under any terms within the written consent of the developer.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * You should have recieved a copy of the rights associated with this program and
 * all associated source files and hardware related to this program.  If you do
 * not agree to these rights, please cease all use and dispose of this program
 * immidately.
 * 
 * Violation of these terms is prosecutable by law.
 *
 *
 * Author: Paimon Sorornejad
 * Version: v1.00
 * Revision Date: 1/31/2012
 * Description: A set of functions designed to handle all clock related
 *              functions of the RxDuino
 ************************************************************************/

#include "delay_methods.h"
#include "debug_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

// Init timer
static unsigned long timer = 0;

// Delay Control
static boolean isOn = false;

/** 
 * Simulate a hardware delay without actually halting the system
 * @param milliDelay	The number of millis to delay for
 * @return				True if delay met
 */
boolean DelayMethods::delay(const unsigned long& milliDelay) {
	if(!isOn) {
		isOn = true;
		timer = millis();
	} else {
		if(timer + milliDelay <= millis()) {
			isOn = false;
			timer = 0;
			return true;
		}
	}	
	
	return false;
}