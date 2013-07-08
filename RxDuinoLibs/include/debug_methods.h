#ifndef _DEBUG_METHODS_
#define _DEBUG_METHODS_

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

#define _DEBUG
 /**
 * Static methods to handle some common tasks
 */
class DebugMethods 
{

public:
static void serialDebug(const char* fmt, ...);
static void dumpMessage(const Frame& message, const char* msg);
};

#endif