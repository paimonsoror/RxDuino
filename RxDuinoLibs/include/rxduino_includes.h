#ifndef _RXDUINO_INCLUDES_
#define _RXDUINO_INCLUDES_

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
