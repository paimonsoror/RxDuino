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
