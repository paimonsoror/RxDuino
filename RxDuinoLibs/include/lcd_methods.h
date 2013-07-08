#ifndef _LCD_METHODS_
#define _LCD_METHODS_

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
 
 /**
 * Static methods to handle some common tasks
 */
class LCDMethods 
{

public:

static void setLcdMessage(const char* data, MCP2515& MSCAN, const int& currentMode);
static void setLcdMessage(const char* data, MCP2515& MSCAN, const int& currentMode, const int& ttime);
static void setLcdMessage(String& data, MCP2515& MSCAN, const int& currentMode);
static void setLcdMessage(const char* custom1, const char* custom2, MCP2515& MSCAN, const int& currentMode);
static void setLcdMessage(const char* screen1, String& screen2, MCP2515& MSCAN, const int& currentMode);
};

#endif
