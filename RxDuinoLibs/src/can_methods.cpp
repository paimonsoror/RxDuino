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
 * Description: A set of libraries that are used to help perform common
 *              CANBUS related tasks
 ************************************************************************/
#include "rxduino_defines.h"
#include "rxduino_methods.h"

#include "can_methods.h"
#include "debug_methods.h"

/**
 * Prepare common fields regardless of type of 
 * CAN message being sent
 *
 * @param  request  The CAN pid that we are requesting
 * @param  message  The message object to prepare
 */
void CANMethods::prepareCommonMessage(const unsigned int& request, Frame& message) {
  message.adrsValue = request;
  message.rtr = false;
  message.dataLength = 8;
  message.isExtendedAdrs = false;
}

/**
 * Prepare an engine ECU message by sending a specific
 * pid to the ECU pid request address
 *
 * @param pid      The pid to request
 * @param message  The CAN message to prepare
 */
void CANMethods::prepareHSCANMessage(const unsigned char& pid, Frame& message) {
  // Prepare message
  prepareCommonMessage(PID_REQUEST, message);
  message.data[0] = 0x02;
  message.data[1] = 0x01;
  message.data[2] = pid;
  for(int i = 3; i <= 7; i++)
    message.data[i] = 0x00;
}

/**
 * Prepare an engine LCD message by sending an empty
 * CAN message to the LCD address
 *
 * @param message  The CAN message to prepare
 */
void CANMethods::initLcdForMessaging(Frame& message) {
  // Prepare message
  prepareCommonMessage(LCD_REQUEST, message);
  for (int i = 0; i <= 7; i++)
    message.data[i] = 0x00; 
}

/**
 * Send an engine LCD message by sending a CAN
 * message filled with bytes of text 
 * to the LCD address
 *
 * @param message  The CAN message to prepare
 * @param screen   The screen to print to
 * @param data     The data to print out
 */
void CANMethods::prepareMSCANMessage(Frame& message, const int& screen, const char* data) {
	
  int maxFit = 6;
  
  //last 2 bytes in 0x291 field are unknown so we stop short
  if(screen == 1) {
	  maxFit = 5;
  }	  
   
  /**
   * Here we are converting the char data to 
   * ASCII chars.  We also want to make sure
   * we clear the rest of the buffer once we
   * hit a null char
   */
  for(int i = 0; i <= maxFit; i++) {
    if(data[i] != '\0') {
      if(data[i] == '*')
        message.data[i + 1] = 0xdf;
      else
        message.data[i + 1] = (int)data[i];
    } else
      for(int j = i; j <= maxFit; j++, i++)
        message.data[j + 1] = 0x20;
  }
  
  /**
   * Depending on the screen, change the address
   */  
  if(screen == 0) {
    // Prepare message
    prepareCommonMessage(LCD_1, message);
    message.data[0] = LCD_1_INIT;
  } else if (screen == 1) {
    // Prepare message
    prepareCommonMessage(LCD_2, message);
    message.data[0] = LCD_2_INIT;
  }
}

