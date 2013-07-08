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
 * Description: Functions that are designed to send messages to the LCD
 *              in the center console of the RX8
 ************************************************************************/
#include "lcd_methods.h"

#include "can_methods.h"
#include "debug_methods.h"

#include "rxduino_defines.h"
#include "rxduino_methods.h"

/**
 * Send an engine LCD message by sending a CAN
 * message filled with bytes of text to the 
 * correct LCD address.  This method will split
 * the string up into two parts to display
 * properly
 *
 * @param  data        The data to print out
 * @param  MSCAN       Reference to the MSCAN bus
 * @param  currentMode The current application mode
 */ 
void LCDMethods::setLcdMessage(String& data, MCP2515& MSCAN, const int& currentMode) {
  char cData[data.length() + 1];
  data.toCharArray(cData, sizeof(cData));
  LCDMethods::setLcdMessage(cData, MSCAN, currentMode);  
}

/** 
 * Send a message to the lcd screen with an arbitrary "timer"
 * to send repeated messages thus simulating a delay
 *
 * @param	data	    The data to print out
 * @param	MSCAN	    Reference to the MSCAN bus
 * @param	currentMode The current application mode
 */
void LCDMethods::setLcdMessage(const char* data, MCP2515& MSCAN, const int& currentMode, const int& ttime) {
	for(int i = 0; i < ttime; i++) {
		setLcdMessage(data, MSCAN, currentMode);
	}
}	

/**
 * Send an engine LCD message by sending a CAN
 * message filled with bytes of text to the 
 * correct LCD address.  This method will split
 * the string up into two parts to display
 * properly
 *
 * @param  data        The data to print out
 * @param  MSCAN       Reference to the MSCAN bus
 * @param  currentMode The current application mode
 */ 
void LCDMethods::setLcdMessage(const char* data, MCP2515& MSCAN, const int& currentMode) {
  
  char screen1[8], screen2[6];
  int totalScreen1Length, totalScreen2Length;
  
  // Set up screen 1
  totalScreen1Length = strlen(data) >= 7? 
                                          7 : strlen(data);
  strncpy(screen1, data,   totalScreen1Length);
  screen1[totalScreen1Length] = '\0';  

  // Set up screen 2 if needed
  if(strlen(data) > 7) {                                              
    totalScreen2Length = strlen(data) - 7 >= 5? 
                                          5 : strlen(data) - 7;       
    strncpy(screen2, data+7, totalScreen2Length);
    screen2[totalScreen2Length] = '\0';
  } else
    // If not, just null out screen2
    screen2[0] = '\0';
  
  // Set the message
  LCDMethods::setLcdMessage(screen1, screen2, MSCAN, currentMode);
  
  // Clean up
  //memset(screen1, 0, sizeof(screen1));
  //memset(screen2, 0, sizeof(screen2));
}

/**
 * Perform MSCAN operations.  For Mazda's the MSCAN contains
 * peripheral information.  Utilizing a 125kbps bus speed,
 * this method will control the MSCAN SPI toggles and send
 * and receive data
 *
 * @param screen1    Message for screen 1
 * @param screen2    Message for screen 2
 * @param MSCAN      Reference to the MSCAN bus
 * @param currentMode The current application mode
 */
void LCDMethods::setLcdMessage(const char* screen1, String& screen2, MCP2515& MSCAN, const int& currentMode) {
  char cScreen2[screen2.length() + 1];
  screen2.toCharArray(cScreen2, sizeof(cScreen2));
  LCDMethods::setLcdMessage(screen1, cScreen2, MSCAN, currentMode);
}

/**
 * Perform MSCAN operations.  For Mazda's the MSCAN contains
 * peripheral information.  Utilizing a 125kbps bus speed,
 * this method will control the MSCAN SPI toggles and send
 * and receive data
 *
 * @param custom1  Message for screen 1
 * @param custom2  Message for screen 2
 * @param MSCAN    Reference to the MSCAN bus
 * @param currentMode The current application mode
 */
void LCDMethods::setLcdMessage(const char* custom1, const char* custom2, MCP2515& MSCAN, const int& currentMode) {
  
  RxduinoMethods::enableMSCAN();
  
  //CAN Structure
  Frame message; 
 
  DebugMethods::serialDebug("|%s||%s|\n", custom1, custom2);   

#ifdef _DEBUGDATA  
  return;
#else	   

  // Prepare & Send Init message
  CANMethods::initLcdForMessaging(message);        
  MSCAN.transmitCANMessage(message, 10);
  CANMethods::prepareMSCANMessage(message, 0, custom1);
  MSCAN.transmitCANMessage(message, 10);
  CANMethods::prepareMSCANMessage(message, 1, custom2);
  MSCAN.transmitCANMessage(message, 10);			  

#endif
  
  RxduinoMethods::disableCAN();
}