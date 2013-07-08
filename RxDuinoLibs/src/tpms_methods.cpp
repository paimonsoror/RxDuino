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
