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

