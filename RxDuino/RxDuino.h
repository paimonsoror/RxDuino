#ifndef _RXDUINO_
#define _RXDUINO_

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

#include "Arduino.h" 
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <stdlib.h>

#include <MCP2515.h>
#include <MCP2515_defs.h>

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "rxduino_defines.h"
#include "rxduino_includes.h"
#include "rxduino_methods.h"

#include "can_methods.h"
#include "canbus_message.h"
#include "clock_methods.h"
#include "cruisecontrol_methods.h"
#include "debug_methods.h"
#include "delay_methods.h"
#include "lcd_methods.h"
#include "program_memory.h"
#include "tpms_methods.h"
#include "mil_methods.h"

// Init current monitor text
static char currentMonitorText[9];
static char currentModeText[12];
static int  currentMonitor;
static int  currentMode;
static int  currentMonitorPid;

// Init current tire pressure
static int tireNumber = 1;

// Init current mil code
static int milCode = 1;

// Failure to detect cruise
static boolean ccFail = false;

// Last button pressed
static unsigned long lastButtonPressedCnt = 0;
static bool holdDownDone = false;

/** 
 * Structure used to store the saved variables in the EEPROM.
 */
struct  savedvars {
	/**
	 * Preamble used signify when the structure has been initialized
	 */
    uint16_t    preamble;
	
	/**
	 * Stored value for the refresh delay
	 */
	int         delay;
	
	/**
	 * The saved display text
	 */
    char		stored[TEXTMAX];
};
extern  volatile    struct savedvars sv;

// Define function prototypes
void setup(void);
void initVars(void);
void initBusses(void);
void loop(void);
void readCruiseControlSwitch(void);
void joystickHandler(const int& action, boolean& buttonPressed);
boolean getEngineData(const unsigned int& pid, const int& byteOverride);
void saveStoredValues(void);
void parseSerialMessage(const String& msg);
int getAlternateState(void);

#endif /* _RXDUINO_ */