#ifndef _CANBUS_MESSAGE_
#define _CANBUS_MESSAGE_

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

class CanbusMessage {
	
public:

static bool getEngineData(const unsigned int& pid, String& engineData, 
						  MCP2515& HSCAN, const int& byteOverride = 0);
static bool getGenericMessage(const byte& responsePid, const float& value, String& engineData);	
static bool getMazdaMessage(const unsigned int& pid, const Frame& message, String& engineData);
static float parseMessage(const Frame& message, const int& bytes);	
static int getByteCount(const unsigned int& pid);
static void createMessage(const unsigned int& pid, Frame& message);
static bool getMilesPerGallon(String& engineData, float& avgMpg, const int& currentMode, MCP2515& HSCAN, MCP2515& MSCAN);
};

#endif /* CANBUS_MESSAGE_H_ */