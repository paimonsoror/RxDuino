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
 * Description: A set of functions designed to handle all tasks related
 *              to the tire pressure monitoring system
 ************************************************************************/
 
#include "mil_methods.h"
#include "debug_methods.h"
#include "lcd_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

/**
 * Function to clear the MIL codes that are currently being thrown 
 * or stored on the vehicle
 * @param HSCAN		Pointer to HSCAN object
 * @param MSCAN		Pointer to MSCAN object
 */
void MILMethods::clearMILCodes(MCP2515& HSCAN, MCP2515& MSCAN) {
  Frame message;
  message.adrsValue = PID_REQUEST;
  message.isExtendedAdrs = false;
  message.rtr = false;
  message.dataLength = 8;
  message.data[0] = 0x02;
  message.data[1] = 0x04;
  for(int i = 2; i <= 7; i++)
	message.data[i] = 0x00;
 
  RxduinoMethods::enableHSCAN(); 
  HSCAN.transmitCANMessage(message, 100);
  RxduinoMethods::disableCAN();
  
  // Post to screen
  LCDMethods::setLcdMessage("Clear: ", "Done", MSCAN, CEL);		
}

/** 
 * Request the total number of MIL codes thrown by the system
 * @param	HSCAN	Pointer to HSCAN Object
 * @return			Number of MIL Codes
 */
int MILMethods::getMILCount(MCP2515& HSCAN) {
	Frame message;
	message.adrsValue = PID_REQUEST;
	message.isExtendedAdrs = false;
	message.rtr = false;
	message.dataLength = 8;
	message.data[0] = 0x01;
	message.data[1] = 0x01;
	for(int i = 2; i <= 7; i++)
	  message.data[i] = 0x00;
	  
	RxduinoMethods::enableHSCAN();
	HSCAN.transmitCANMessage(message, 100);
	RxduinoMethods::disableCAN();
	
	
	// 1000 | 0000 = x80
	// 0111 | 1111 = x7F
	//A7654   3210
	bool milOn = (message.data[2] & 0x80);
	int milCount = (message.data[2] & 0x7F);
	
	return milCount;	
}

/**
 * Prepare Message for grabbing the MIL Code
 * @param HSCAN		Pointer to HSCAN object
 * @param MSCAN		Pointer to MSCAN object
 * @param milIndex  The index of the MIL code
 * @return        True if valid data is returned
 */
boolean MILMethods::getMILData(MCP2515& HSCAN, MCP2515& MSCAN, int& milIndex) {
 
  Frame message;
  message.adrsValue = PID_REQUEST;
  message.isExtendedAdrs = false;
  message.rtr = false;
  message.dataLength = 8;
  
#ifdef _DEBUGDATA  
  // If debug data is enabled, make up some arbitrary 
  // values here for a MIL code
  // 7E8 4 43 1 1 37 0 0 0
  message.adrsValue = 0x7E8;
  message.data[0] = 0x04;
  message.data[1] = 0x43;
  message.data[2] = 0x01;
  message.data[3] = 0x37;
  message.data[4] = 0x01;
  message.data[5] = 0x43;
  for(int i = 6; i < 8; i++) {
    message.data[i] = 0x00;
  }   
  
  int mCount = 2;
#else
/*
  message.data[0] = 0x02;
  message.data[1] = 0x03;
  for(int i = 2; i <= 7; i++)
	message.data[i] = 0x00;	*/

  message.data[0] = 0x03;
  for(int i = 1; i <= 7; i++)
  message.data[i] = 0x00;
	
  // Find total number of codes
  int mCount = 1; //MILMethods::getMILCount(HSCAN);
  
  DebugMethods::serialDebug("Total MIL: %d\n", mCount);
	
  RxduinoMethods::enableHSCAN(); 
  
  if(HSCAN.transmitCANMessage(message,100)) {
    if(HSCAN.receiveCANMessage(message, 100)) {    
#endif

	  // If we chose an index greater than the total
	  // mil codes, set the milIndex back to 1
	  if(milIndex > mCount) milIndex = 1;
	  
	  DebugMethods::dumpMessage(message, "MIL:");
	  if(message.data[1] == 0x43) {

		//for(int i = 0; i < mCount; i++) {
			int hexA = message.data[2 + (milIndex - 1)];
			int hexB = message.data[2 + milIndex];
		   
			String milCode = ""; 
			switch(hexA & 0xC0)
			{
				case 0x00:
				milCode += 'P';  // powertrain
				break;
			
				case 0x40:
				milCode += 'C';  // chassis
				break;
			
				case 0x80:
				milCode += 'B';  // body
				break;
			
				case 0xC0:
				milCode += 'U';  // network
				break;
			}
		
			// How a DTC is formatted
			// 00 | 00 | 0001 || 0100 | 0011
			//        x              x+1 
			//
			// First 2 bits of message x indicate P
			// second 2 bits indicate a 0
			// next 4 bits indicates 1
			// next 4 indicate 4
			// next 4 indicate 3
		  
			milCode += String((hexA & 0x30) >> 4, DEC);   // first digit is 0-3 only
			milCode += String((hexA & 0x0F),      DEC);
			milCode += String((hexB & 0xF0) >> 4, DEC);
			milCode += String((hexB & 0x0F),      DEC);
			milCode.trim();
		   
			// Post to screen
			if(!milCode.equals("P0000"))
				LCDMethods::setLcdMessage("MIL:", milCode, MSCAN, CEL);
				
		//}				
			
		RxduinoMethods::disableCAN();
		   		   
		return true;
      
	  } else {
		// Post to screen
		LCDMethods::setLcdMessage("MIL:", "None", MSCAN, CEL);
		RxduinoMethods::disableCAN();
		
		return false;
	  }
#ifndef _DEBUGDATA   
    }
  }  
  
  RxduinoMethods::disableCAN();
  return false;
#endif
}