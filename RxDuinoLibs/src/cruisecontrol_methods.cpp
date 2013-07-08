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
 * Description: Functions that are used to handle all cruse control tasks
 *              for the RxDuino
 ************************************************************************/
#include "cruisecontrol_methods.h"
#include "debug_methods.h"
#include "lcd_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

/**
 * Get the current CC button that is pressed
 * @param message  The message object to be manipulated
 * @param HSCAN    The HSCAN class
 * @return		   True if the CC button was properly read
 */
boolean CruiseControlMethods::getCCButton(Frame& message, MCP2515& HSCAN) {
  
  message.adrsValue = CC_REQUEST;
  message.isExtendedAdrs = false;
  message.rtr = false;
  message.dataLength = 8;
  message.data[0] = 3;
  message.data[1] = 0x22;
  message.data[2] = 0xA2;
  message.data[3] = 0x16;
  for(int i = 4; i < 8; i++) {
    message.data[i] = 0x00;
  } 
  
  RxduinoMethods::enableHSCAN();
  boolean value = HSCAN.queryOBDExtended(CC_REPLY, message);
  RxduinoMethods::disableCAN();
  
  return value; 
}

/**
 * Get the status of the CC
 * @param message  The message object to be manipulated
 * @param HSCAN    The HSCAN class
 * @return		   True if the CC is on
 */
boolean CruiseControlMethods::getCCStatus(MCP2515& HSCAN, boolean& failure) {
  boolean value = false;
  
  Frame message;
  message.adrsValue = CC_ONOFF;
  message.isExtendedAdrs = false;
  message.rtr = false;
  message.dataLength = 8;
  message.data[0] = 3;
  message.data[1] = 0x22;
  for(int i = 2; i < 8; i++) {
    message.data[i] = 0x00;
  } 
   
  RxduinoMethods::enableHSCAN();

  for(int i = 0; i < CC_POLLING; i++) {
	if(HSCAN.receiveCANMessage(message, 100)) {
      if(message.adrsValue == 0x650) {
        if(message.data[0] == 0x80 || message.data[0] == 0xFF) {
		  failure = false;
          value = true;
		  break;
		}		  
	  }
	} else {
		failure = true;
		break;
	}			 		
   }  
				   
  RxduinoMethods::disableCAN();	
  
  // Force false cc failure if in debug mode
  #ifdef _DEBUGDATA	
  failure = false;
  #endif
  
  return value;
}
