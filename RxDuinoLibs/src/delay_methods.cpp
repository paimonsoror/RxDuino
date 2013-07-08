/************************************************************************
 * RXDUINO Software, All Rights Reserved 2012
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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