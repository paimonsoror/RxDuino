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
 * Description: Common methods for the RxDuino
 ************************************************************************/

#include "rxduino_defines.h"
#include "rxduino_methods.h"

/**
 * Initialize all hardware devices.
 */
void RxduinoMethods::init_all(void)
{	 	
  init_SPI();
  init_Status_LED();
  
  // Turn off adc
  cbi(ADCSRA,ADEN);
}

/**
 * Initialize the SPI pins for both CAN busses
 */
void RxduinoMethods::init_SPI(void)
{
  // Begin the SPI module
  SPI.setClockDivider(SPI_CLOCK_DIV2);   // setClockDivider divide by 2 since MCP2515 runs its SPI at 10Mhz.
  SPI.setDataMode(SPI_MODE0);            // DataMode 0 (0,0) or 3 (1,1) are the only ones to use with the MCP2515
  SPI.setBitOrder(MSBFIRST);             // MSB as used by the MCP2515
  SPI.begin();
  
  pinMode(HSCANCS,OUTPUT);				 // Initialize the CAN bus card SPI CS Pin
  digitalWrite(HSCANCS, HIGH);
  pinMode(MSCANCS,OUTPUT);
  digitalWrite(MSCANCS, HIGH);
  
  disableCAN();
}  

/**
 * Initialize the status LED
 */
void RxduinoMethods::init_Status_LED(void)
{
  pinMode(LED_2, OUTPUT); /*Status LED*/
  pinMode(LED_3, OUTPUT); /*communication LED*/
  digitalWrite(LED_2,HIGH);
}

/** 
 * Enable the HSCAN by setting the MSCAN pin
 * low and the MSCAN high
 */
void RxduinoMethods::enableHSCAN() {
  digitalWrite(MSCANCS, HIGH);
  digitalWrite(HSCANCS, LOW); 
}

/**
 * Enable the MSCAN by setting the HSCAN pin
 * low and the HSCAN high
 */
void RxduinoMethods::enableMSCAN() {
  digitalWrite(HSCANCS, HIGH);
  digitalWrite(MSCANCS, LOW); 
}

/**
 * Disable all CAN devices by setting their
 * pins high
 */
void RxduinoMethods::disableCAN() {
  digitalWrite(HSCANCS, HIGH);
  digitalWrite(MSCANCS, HIGH); 
}