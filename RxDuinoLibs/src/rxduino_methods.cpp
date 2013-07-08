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