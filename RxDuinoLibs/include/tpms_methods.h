#ifndef _TPMS_METHODS_
#define _TPMS_METHODS_

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
 ************************************************************************/
 #include <MCP2515.h>
 #include <MCP2515_defs.h>

 #define boolean bool
 
 /**
 * Static methods to handle tpms tasks
 */
class TPMSMethods 
{
  public:
  
  static boolean getTPMData(const int& number, const unsigned int& msb, const unsigned int& lsb, Frame& message, MCP2515& HSCAN);
  static void getWheelTemp(const int& number, const int& currentMode, MCP2515& MSCAN, MCP2515& HSCAN);
  static void getWheelPSI(const int& number, const int& currentMode, MCP2515& MSCAN, MCP2515& HSCAN);
  
};

#endif
