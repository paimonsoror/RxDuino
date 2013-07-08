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
 * Description: A set of functions designed to handle all tasks related
 *              to the tire pressure monitoring system
 ************************************************************************/
 
#include "tpms_methods.h"
#include "debug_methods.h"
#include "lcd_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

/**
 * Prepare Message for grabbing the wheel PSI
 * @param number		The wheel number to query
 * @param currentMode	The current display mode
 * @param MSCAN			Pointer to the MSCAN object
 * @param HSCAN			Pointer to the HSCAN object
 */
void TPMSMethods::getWheelPSI(const int& number, const int& currentMode, MCP2515& MSCAN, MCP2515& HSCAN) {
  Frame message;
  if(getTPMData(number, 0xC9, 0x01, message, HSCAN)) {   
    char s1buff[SCREEN1MAX];
    char s2buff[SCREEN2MAX];
    int respData = (( message.data[3 + number] * 1373.) / 1000.) * 0.145037738;
    sprintf(s1buff, "Tire%d:", number);
    sprintf(s2buff, "%d", respData);
    LCDMethods::setLcdMessage(s1buff, s2buff, MSCAN, currentMode);  
  }
}

/**
 * Prepare Message for grabbing the wheel PSI
 * @param number  The wheel number to query
 * @param currentMode	The current display mode
 * @param MSCAN			Pointer to the MSCAN object
 * @param HSCAN			Pointer to the HSCAN object
 */
void TPMSMethods::getWheelTemp(const int& number, const int& currentMode, MCP2515& MSCAN, MCP2515& HSCAN) {
  Frame message;
  if(getTPMData(number, 0xC9, 0x02, message, HSCAN)) {
    char s1buff[SCREEN1MAX];
    char s2buff[SCREEN2MAX];
    int respData = ((9/5) * (message.data[3 + number] - 50)) + 32;
    sprintf(s1buff, "Tire%d:", number);
    sprintf(s2buff, "%d*F", respData);
    LCDMethods::setLcdMessage(s1buff, s2buff, MSCAN, currentMode);  
  }
}

/**
 * Prepare Message for grabbing data from the TPM ECU
 * @param number  The wheel number to query
 * @param msb     The most significant bits
 * @param lsb     The least signigicant bits
 * @param message The CAN message object
 * @param HSCAN	  Pointer to the HSCAN object
 * @return        True if valid data is returned
 */
boolean TPMSMethods::getTPMData(const int& number, const unsigned int& msb, const unsigned int& lsb, Frame& message, MCP2515& HSCAN) {
  message.adrsValue = TPM_REQUEST;
  message.isExtendedAdrs = false;
  message.rtr = false;
  message.dataLength = 8;
  message.data[0] = 3;
  message.data[1] = 0x22;
  message.data[2] = msb;
  message.data[3] = lsb;
  for(int i = 4; i < 8; i++) {
    message.data[i] = 0x00;
  } 
  
  RxduinoMethods::enableHSCAN();
  return HSCAN.queryOBDExtended(TPM_REPLY, message);
  RxduinoMethods::disableCAN();
}
