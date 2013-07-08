#ifndef _CANBUS_MESSAGE_
#define _CANBUS_MESSAGE_

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