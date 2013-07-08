#ifndef _RXDUINO_INCLUDES_
#define _RXDUINO_INCLUDES_

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

#include "rxduino_defines.h"

MCP2515 HSCAN;
MCP2515 MSCAN;

// Welcome Message
const char startText[TEXTMAX] = "RxDUINOv1.02";

// Default text
const char defaultText[TEXTMAX] = "Rotary Power";

// Total Modes and Monitors avaiable
const int totalModes = 15,
          totalMonitors = 15;

// Initialize Monitor Text                       
const char rpmLabel[] PROGMEM		= "RPM:";
const char engTempLabel[] PROGMEM	= "EngTmp:";
const char speedLabel[] PROGMEM		= "Speed:";
const char mafLabel[] PROGMEM		= "MAF:";
const char tpsLabel[] PROGMEM		= "TPS:";
const char wbafrLabel[] PROGMEM		= "WBAFR:";
const char olafrLabel[] PROGMEM		= "OLAFR:";
const char loadLabel[] PROGMEM		= "Load:";
const char catLabel[] PROGMEM		= "CatTmp:";
const char iatLabel[] PROGMEM		= "IAT:";
const char stftLabel[] PROGMEM		= "STFT:";
const char ltftLabel[] PROGMEM		= "LTFT:";
const char runtLabel[] PROGMEM		= "RunT:";
const char fuelLabel[] PROGMEM		= "Fuel:";
const char controlLabel[] PROGMEM	= "Contrl:";  

// Initialize Mode Text
const char ecuMonitorText[] PROGMEM		= "ECU Monitor";
const char milesPerGalText[] PROGMEM	= "MilesPerGal";
const char avgMpgText[] PROGMEM			= "AvgMPG";
const char wheelSpeedText[] PROGMEM		= "Wheel Speed";
const char wheelPsiText[] PROGMEM		= "Wheel Press";
const char wheelTempText[] PROGMEM		= "Wheel Temp";
const char textModeText[] PROGMEM		= "Text Mode";
const char askClockText[] PROGMEM		= "Set Clock";
const char troubleCodesText[] PROGMEM	= "TroubleCodes";
const char hsSniffText[] PROGMEM		= "HSSniff";
const char msSniffText[] PROGMEM		= "MSSniff";
const char milClearText[] PROGMEM		= "MIL Clear";
const char systemOffText[] PROGMEM		= "System Off";
const char respTimeText[] PROGMEM		= "RespTime";
const char editTextText[] PROGMEM		= "Edit Text";
const char setClockText[] PROGMEM		= "Setting Clk";

// Display Text
static char dispText[TEXTMAX];
static int  dispTextIndex = 0;

// Custom Refresh
static int customDelay = 0;

// Storage for engine data
//char engineData[8];
static String engineData;

// Avg MPG
static float avgMpg = 0;
    
#endif
