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
 * Description: This application is designed for the Series 2 Mazda 
 *              RX8 (2009-2011).  The name of the application is The
 *              RxDuino.  The intent of the RxDuino application is to
 *              be installed on an arduino based device, where it will 
 *              connect to the vehicle via the OBD2 port.  Once connected
 *              it will log information from the available ECU's and 
 *              display them on the information display in the center
 *              console.
 ************************************************************************/
#include "RxDuino.h"

//////////////////////////////////////////////////////////////////////////////////
// Nightly or International Defines
//////////////////////////////////////////////////////////////////////////////////

#ifdef _NIGHTLY
#ifdef _INTERNATIONAL
char intStr[15] = "International ";
#else
char intStr[1] = "";
#endif
char nightlyString[50];
#endif

//////////////////////////////////////////////////////////////////////////////////
// EEPROM Struct Code
//////////////////////////////////////////////////////////////////////////////////

volatile struct savedvars sv;

//////////////////////////////////////////////////////////////////////////////////
// Executable Code
//////////////////////////////////////////////////////////////////////////////////
          
/**
 * Prepare the RXDUINO application.  This method is called when the
 * device is first booted up
 */
void setup(void)
{      
  Serial.begin(115200);
  
  // Initialize SPI
  RxduinoMethods::init_all();
  
  // Init all vars  
  initVars();
  
  // Initialize CAN Busses
  initBusses();
}

/**
 * Initialize the can based busses
 */
void initBusses(void) {
	if(HSCAN.initCAN(CAN_BAUD_500K, HSCANCS)) {
		DebugMethods::serialDebug("HSCAN Initialized\n");
		  
		// Once initialize set the mode to normal
		if(!HSCAN.setMode(NORMAL)) {
			DebugMethods::serialDebug("Error Setting HSCAN Normal\n");
		}
		  
		// Initialize The MSCAN
		if(MSCAN.initCAN(CAN_BAUD_125K, MSCANCS)){
			DebugMethods::serialDebug("MSCAN Initialized\n");
			  
			// Set mode to normal
			if(!MSCAN.setMode(NORMAL)) {
				DebugMethods::serialDebug("Error Setting MSCAN Normal\n");
			}
			  
			DebugMethods::serialDebug("Printing Welcome Message\n");
			LCDMethods::setLcdMessage( startText, MSCAN, currentMode);
			delay(1000);
			  
		} else {
			DebugMethods::serialDebug("RxDUINO::MSCAN Initialize Failure, shut down\n");
		}
	} else {
		DebugMethods::serialDebug("RxDUINO::HSCAN Initialize Failure, shut down\n");
	}
}

/**
 * Initialize all variables used in the application
 */
void initVars(void)
{  
  // Message initializing
  DebugMethods::serialDebug("Initializing...", true);

  // Set current Monitor
  currentMonitor = 0;
  
  // Init Clock Vars
  ClockMethods::initClockVars();
  
  // Get current monitor pid
  currentMonitorPid = pgm_read_byte(&allMonitors[currentMonitor]);
  DebugMethods::serialDebug("Current Monitor Pid: %d\n", currentMonitorPid);
  
  // Set current Mode
  currentMode = ECU;
  
  // Get current monitor text
  strcpy_P(currentMonitorText, (char*)pgm_read_word(&(allMonitorsText[0])));
  DebugMethods::serialDebug("Current Monitor Text: %s\n", currentMonitorText);
  
  // Get current mode text
  strcpy_P(currentModeText, (char*)pgm_read_word(&(allModesText[0])));
  DebugMethods::serialDebug("Current Mode Text: %s\n", currentModeText);
  
  // Read stored text from our EEPROM for user info  
  eeprom_read_block((void *) &sv, 0, sizeof(sv));
 
  // Check the preamble for our random value, if this is not set then
  // there is no eeprom initialized
  if(sv.preamble != _PREAMBLE_) {
	  sv.preamble = _PREAMBLE_;
	  sv.delay = 100;
	  strcpy((char *)sv.stored, defaultText);
  }
  strcpy(dispText, (char *)sv.stored);
  DebugMethods::serialDebug("Display Text: %s\n", dispText);
    
  // Read custom Refresh 
  customDelay = sv.delay;
  DebugMethods::serialDebug("Custom Delay: %d\n", customDelay);
  
  // Are we in debug mode?
#ifdef _DEBUGDATA
  DebugMethods::serialDebug("Note: Running In Debug Mode\n");
#endif

  // Nightly??
#ifdef _NIGHTLY
  sprintf(nightlyString, "%s Nightly: 1/10, 10:30pm", intStr);
  DebugMethods::serialDebug("%s\n", nightlyString);
#endif
}

/**
 * Standard Arduino loop method.  Loops application
 * indefinitely
 */
void loop(void)
{   
  String txtMsg = "";
  
  // Did the user press anything?
  readCruiseControlSwitch();

#ifdef _ALLOWINPUT  
  // Is there a message on the bus
  // add any incoming characters to the String:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    txtMsg += inChar;
  }
  
  if(txtMsg.compareTo("") != 0) {
	  parseSerialMessage(txtMsg);
  }
#endif

  // Check Failure
  if(currentMode != OFF && !ccFail) {  
    switch(currentMode) {
      case ECU:
	  CanbusMessage::getEngineData(currentMonitorPid, engineData, HSCAN);	
	  LCDMethods::setLcdMessage(currentMonitorText, engineData, MSCAN, currentMode); 
      break;
      
      case MILES_PER_GAL:
	  case AVG_MPG:
      if(CanbusMessage::getMilesPerGallon(engineData, avgMpg, currentMode, HSCAN, MSCAN)) {
		  LCDMethods::setLcdMessage(engineData, MSCAN, currentMode);
	  }
      break;
      
      case WHEEL_SPEED:
      if(CanbusMessage::getEngineData(ID_4B0, engineData, HSCAN)) {
        LCDMethods::setLcdMessage(engineData, MSCAN, currentMode);
      }
      break;
      
      case WHEEL_PSI:
      TPMSMethods::getWheelPSI(tireNumber, currentMode, MSCAN, HSCAN);
      break;
      
      case WHEEL_TEMP:
      TPMSMethods::getWheelTemp(tireNumber, currentMode, MSCAN, HSCAN);
      break;
        
      case TEXT:
      case EDIT_TEXT:
      LCDMethods::setLcdMessage(dispText, MSCAN, currentMode);
      break;
      
      case ASK_CLOCK:
	  ClockMethods::getInitialClock(MSCAN);
	  break;
	  
	  case SET_CLOCK:
	  ClockMethods::setClock(MSCAN);
      currentMode = ASK_CLOCK;
      break;
      
      case CEL:
	  MILMethods::getMILData(HSCAN, MSCAN, milCode);
      break;
	  
	  case MIL_CLEAR:
	  LCDMethods::setLcdMessage("ClrMIL(DN)", MSCAN, currentMode);
	  break;
         
      case RESPONSE_TIME:
      char buf[5];
      sprintf(buf, "%d", customDelay);
      LCDMethods::setLcdMessage(buf, "ms.", MSCAN, currentMode);
      break;
    }
  }
}

/** 
 * Parse the message on the serial bus
 * @param msg	The message on the bus
 */
#ifdef _ALLOWINPUT
void parseSerialMessage(const String& msg) {
	
	// Control for button press
	boolean buttonPressed = false;
	
	if(msg.compareTo("RST") == 0) {
		asm volatile ("  jmp 0");
	} else if(msg.substring(0,3) == "TXT") {
		char tmp[msg.length()];
		msg.toCharArray(tmp, msg.length(), msg.indexOf(" ")+1);
		strcpy(dispText, tmp);
		DebugMethods::serialDebug("Writing To Memory: ", false);
		DebugMethods::serialDebug(dispText, true);
		saveStoredValues();
	} else if(msg.compareTo("NEXT") == 0) {
		joystickHandler(RIGHT, buttonPressed);
	} else if(msg.compareTo("PREV") == 0) {
		joystickHandler(LEFT, buttonPressed);
	} else if(msg.compareTo("UP") == 0) {
		joystickHandler(UP, buttonPressed);
	} else if(msg.compareTo("DOWN") == 0) {
		joystickHandler(DOWN, buttonPressed);			
	} else if(msg.compareTo("ALT") == 0) {
		joystickHandler(CLICK, buttonPressed);
	} else if(msg.compareTo("OFF") == 0) {
		DebugMethods::serialDebug("Shutting Down", true);
		currentMode = OFF;
	} else if(msg.compareTo("PING") == 0) {
		DebugMethods::serialDebug("Serial Communications: OK" , true);
	} else if(msg.compareTo("DUMP") == 0) {
		// Message initializing
		DebugMethods::serialDebug("Current Monitor Pid: %d\n", currentMonitorPid);
		DebugMethods::serialDebug("Current Monitor Text: %s\n", currentMonitorText);
		DebugMethods::serialDebug("Current Mode Text: %s\n", currentModeText);
		DebugMethods::serialDebug("Display Text: %s\n", dispText);
		DebugMethods::serialDebug("Custom Delay: %d\n", customDelay);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////////
// Cruise Control Functions
//////////////////////////////////////////////////////////////////////////////////

/** 
 * Handler for reading the cruise control switch
 */
void readCruiseControlSwitch(void) {
    Frame message;
	
    // Check if CC is on
    boolean ccIsOn = CruiseControlMethods::getCCStatus(HSCAN, ccFail);
    
    if(!ccIsOn) {
      // Query Cruise Control Switch Module
      if(CruiseControlMethods::getCCButton(message, HSCAN)) {
        boolean buttonPressed = false;
		
		// The cruise control can be held down so lets make sure
		// that this is not a hold down and just a single press
		if(message.data[4] != CC_CNCL && lastButtonPressedCnt != 0) {
			joystickHandler(RIGHT, buttonPressed);
			lastButtonPressedCnt = 0;
			holdDownDone = true;
			Serial.println("Timer Ended");
		}	
        
        // Read data
        switch(message.data[4]) {
			case CC_UP: // up
            DebugMethods::serialDebug("CC Up Pressed", true);
            joystickHandler(UP,    buttonPressed);
            break; 
            case CC_DOWN: // down
            DebugMethods::serialDebug("CC Down Pressed", true);
            joystickHandler(DOWN,  buttonPressed);
            break;
            case CC_CNCL: // cancel
            DebugMethods::serialDebug("CC Cancel Pressed", true);
			if (lastButtonPressedCnt == 0) {      // check if continued press
				lastButtonPressedCnt = millis();  // if not, set timer
				Serial.println("Timer Started");
				holdDownDone = false;
			} else if ((millis() - lastButtonPressedCnt) > 1000 && !holdDownDone) { // if so, check if long ennough
				Serial.println("Button Held Down");
				joystickHandler(CLICK, buttonPressed);
				holdDownDone = true;
				lastButtonPressedCnt = 0;
			} // else ignore
            break;
        }
      
	    if(buttonPressed)
		  delay(125);
	  }
		
    } else {
      DebugMethods::serialDebug("Cruise Control Is On, Buttons Disabled", true);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Joystick Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * Handle the operation the user selected from the joystick
 *
 * @param  action        The action the user requested
 * @param  buttonPressed Signal the button pressed flag to
 *                       set the delay on a valid command
 */
void joystickHandler(const int& action, boolean& buttonPressed) {
  switch(action) {
    // User Pressed Down
    case DOWN:
    if(currentMode == ECU) {
      if(currentMonitor < (totalMonitors - 1)) {
        currentMonitor++;   
        DebugMethods::serialDebug("Current Mode: ", false);
        strcpy_P(currentMonitorText, (char*)pgm_read_word(&allMonitorsText[currentMonitor]));
        currentMonitorPid = pgm_read_byte(&allMonitors[currentMonitor]);
        DebugMethods::serialDebug("%s\n", currentMonitorText);        
      }
    } else if (currentMode == EDIT_TEXT) {
      dispText[dispTextIndex] -= 1;  
    } else if (currentMode == RESPONSE_TIME) {
      if( customDelay > 0 ) 
        customDelay-=100;
    } else if (currentMode == ASK_CLOCK) {
      ClockMethods::incrementMin();
	  delay(200);
    } else if (currentMode == WHEEL_PSI || currentMode == WHEEL_TEMP) {
      if(tireNumber < 4) tireNumber++;
	} else if (currentMode == CEL) {
	  if(milCode < 3) milCode++;	  
    } else if (currentMode == MIL_CLEAR) {
	  MILMethods::clearMILCodes(HSCAN, MSCAN);
	}
    buttonPressed = true; 
    break;
    
    // User Pressed Up
    case UP:
    if(currentMode == ECU) {
      if(currentMonitor > 0) {
        currentMonitor--;
        DebugMethods::serialDebug("Current Mode: ", false);
        strcpy_P(currentMonitorText, (char*)pgm_read_word(&allMonitorsText[currentMonitor]));
        currentMonitorPid = pgm_read_byte(&allMonitors[currentMonitor]);
        DebugMethods::serialDebug("%s\n", currentMonitorText);
      }
    } else if (currentMode == ASK_CLOCK) {
      ClockMethods::incrementHour();
	  delay(200);
    } else if (currentMode == EDIT_TEXT) {
      dispText[dispTextIndex] += 1;
    } else if (currentMode == RESPONSE_TIME) {
      customDelay += 100;
    } else if (currentMode == WHEEL_PSI || currentMode == WHEEL_TEMP) {
      if(tireNumber > 1) tireNumber--;
    } else if (currentMode == CEL) {
	  if(milCode > 1) milCode--;
	}		
    buttonPressed = true; 
    break;
    
    // User Pressed Left
    case LEFT:
    if(currentMode == EDIT_TEXT) {
      if(dispTextIndex > 0) {
        dispTextIndex--;
      }
    } else if(currentMode > ECU && currentMode < RESPONSE_TIME) {
      strcpy_P(currentModeText, (char*)pgm_read_word(&allModesText[--currentMode]));
      LCDMethods::setLcdMessage(currentModeText, MSCAN, currentMode, 20);
    }
    buttonPressed = true; 
    break;
    
    // User pressed right
    case RIGHT:
    if(currentMode == EDIT_TEXT) {
      if(dispTextIndex < TEXTMAX) {
        dispTextIndex++;
      }  
    } else if(currentMode < OFF) {
      strcpy_P(currentModeText, (char*)pgm_read_word(&allModesText[++currentMode])); 
      LCDMethods::setLcdMessage(currentModeText, MSCAN, currentMode, 20);
	  delay(100);
    } else if(currentMode == OFF) {
      currentMode = ECU;
      strcpy_P(currentModeText, (char*)pgm_read_word(&allModesText[currentMode]));
      LCDMethods::setLcdMessage(currentModeText, MSCAN, currentMode, 20);
    }
    buttonPressed = true; 
    break;
    
    // User clicked joystick
    case CLICK:
    currentMode = getAlternateState();
    strcpy_P(currentModeText, (char*)pgm_read_word(&allModesText[currentMode]));
    LCDMethods::setLcdMessage(currentModeText, MSCAN, currentMode);//, 100);
    delay(100);
    buttonPressed = true; 
    break;
  }  
}

/** 
 * Report the next state
 * @return The next state based on the current mode
 */
int getAlternateState() {
	if(currentMode == TEXT) {
		return EDIT_TEXT;
	} else if (currentMode == EDIT_TEXT) {
		saveStoredValues();
		return TEXT;
	} else if (currentMode == ECU) {
		return RESPONSE_TIME;
	} else if (currentMode == RESPONSE_TIME) {
		saveStoredValues();
		return ECU;
	} else if (currentMode == ASK_CLOCK) {
		return SET_CLOCK;
	} else if (currentMode == SET_CLOCK) {
		return ASK_CLOCK;
	} else if (currentMode == MILES_PER_GAL) {
		return AVG_MPG;
	} else if (currentMode == AVG_MPG) {
		return MILES_PER_GAL;
	} else if (currentMode == WHEEL_PSI) {
		return WHEEL_TEMP;
	} else if (currentMode == WHEEL_TEMP) {
		return WHEEL_PSI;
	} else if (currentMode == CEL) {
		return MIL_CLEAR;
	} else if (currentMode == MIL_CLEAR) {
		return CEL;
	} else {
		return currentMode;
	}		
}

/**
 * Save the EEPROM values if they have been changed
 */
void saveStoredValues() {
	// If the user changed the delay, save the new delay
	if(sv.delay != customDelay) {
		DebugMethods::serialDebug("User Updated Delay", true);
		sv.delay = customDelay; 
		eeprom_write_block((void *)&sv, 0, sizeof(sv)); 
	}
	
	// if the user changed the text, save the new text
	if(strcmp((char*)sv.stored, dispText) != 0) {
		DebugMethods::serialDebug("User Updated Text", true);
		// Save user changes
		strcpy((char *)sv.stored, dispText);
		eeprom_write_block((void *)&sv, 0, sizeof(sv)); 
	}	 	  
}