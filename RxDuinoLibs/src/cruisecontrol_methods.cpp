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
