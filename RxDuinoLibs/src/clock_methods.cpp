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
#include "clock_methods.h"
#include "can_methods.h"
#include "debug_methods.h"
#include "lcd_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

String ClockMethods::clockH = "12";
String ClockMethods::clockM = "00";

/** 
 * Initialize the clock variables by setting the time to
 * 12:00.
 */
void ClockMethods::initClockVars() {
	clockH = "12";
	clockM = "00";
}

/** 
 * Get the user defined clock time.  This is what he/she is 
 * going to see before they commit the change
 * @param	MSCAN	The MSCAN object
 */
void ClockMethods::getInitialClock(MCP2515& MSCAN) {
	String time = "Set:" + clockH + ":" + clockM;
	LCDMethods::setLcdMessage(time, MSCAN, ASK_CLOCK);
}

/** 
 * Increment the hour, or rollover if greater than 12
 */
void ClockMethods::incrementHour() {
	char chr[3]; clockH.toCharArray(chr, sizeof(chr));
	int hr = atoi(chr);
	if(hr >= 12)
		hr = 1;
	else
		hr++;
	clockH = hr >= 10? String(hr) : String("0" + String(hr));
}

/** 
 * Increment the minutes, or rollover if greater than 59
 */
void ClockMethods::incrementMin(){
	char cmin[3]; clockM.toCharArray(cmin, sizeof(cmin));
	int min = atoi(cmin);
	if(min >= 59)
		min = 00;
	else
		min++;
	clockM = min >= 10? String(min) : String("0" + String(min));
}

/** 
 * Set the clock
 * @param	MSCAN	The MSCAN object
 */
void ClockMethods::setClock(MCP2515& MSCAN){
	// Hours are found by doing hour * 0x08
	char this_char[clockH.length() + 1];
	clockH.toCharArray(this_char, sizeof(this_char));
	uint32_t bhour = atoi(this_char) * 0x800;
	
	// Minutes are min * 0x20
	char this_charm[clockM.length() + 1];
	clockM.toCharArray(this_charm, sizeof(this_charm));
	uint32_t bmin = atoi(this_charm) * 0x20;
	
	// combine them!
	uint32_t hourandmin = bhour + bmin;
	
	DebugMethods::serialDebug("Hour: %d\n", bhour);
	DebugMethods::serialDebug("Minute: %d\n", bmin);
	DebugMethods::serialDebug("Total: %d\n", hourandmin);
	
	// 0x292 message
	// 292	18	FE	9C	59	C0	0	0	0
	Frame message;
	message.adrsValue = 0x292;
	message.isExtendedAdrs = false;
	message.rtr = false;
	message.dataLength = 8;
	message.data[0] = 0x18;
	message.data[1] = 0xFE;
	message.data[2] = hourandmin / 0x100;
	message.data[3] = hourandmin - bhour;
	message.data[4] = 0x40;
	for(int i = 5; i < 8; i++) {
		message.data[i] = 0x00;
	}
	
	DebugMethods::dumpMessage(message, "Clock:");
	MSCAN.transmitCANMessage(message, 100);
}
