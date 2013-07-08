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
 * Description: A set of functions designed to aid the designer in printing
 *              debug messages
 ************************************************************************/

#include "debug_methods.h"

void DebugMethods::serialDebug(const char *fmt, ... ) {
	char tmp[128]; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt );
	vsnprintf(tmp, 128, fmt, args);
	va_end (args);
	Serial.print(tmp);
}

/**
 * Dump the contents of the CAN message
 *
 * @param message  The can message to dump
 * @param msg      Message to be printed before
 */
void DebugMethods::dumpMessage(const Frame& message, const char* msg) {
#ifdef _DEBUG
  Serial.print(msg);
  Serial.print(": ");
  Serial.print(message.adrsValue, HEX);
  Serial.print(" ");
  for(int i = 0; i <= message.dataLength - 1; i++) {
    Serial.print(message.data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("- Ext:");
  if(message.isExtendedAdrs) {
    Serial.print("Yes ");
  } else {
    Serial.print("No ");
  }
  Serial.print("- Len:");
  Serial.println(message.dataLength,DEC);
#endif
}
