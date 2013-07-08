#ifndef _RXDUINO_DEFINES_
#define _RXDUINO_DEFINES_

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
#include <MCP2515.h>
#include <MCP2515_defs.h>

// Enable Debug
#define _DEBUG
//#define _BLUETOOTH
#define _ALLOWINPUT
//#define _DEBUGDATA
#define _CC_ON
#define _NIGHTLY
//#define _INTERNATIONAL

#define _PREAMBLE_ 0x26

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define TEXTMAX    13
#define SCREEN1MAX 8
#define SCREEN2MAX 6

#define DELAYLOCATION 14

/*Operation Modes*/
#define ECU   0
#define                 RESPONSE_TIME 8
#define MILES_PER_GAL 1
#define                 AVG_MPG 9
#define WHEEL_SPEED 2
#define WHEEL_PSI   3
#define                 WHEEL_TEMP 10
#define TEXT  4
#define                 EDIT_TEXT 11
#define ASK_CLOCK 5
#define                 SET_CLOCK 12
#define CEL   6
#define                 MIL_CLEAR 13
//#define HSSNIFF 7
//#define MSSNIFF 8
#define                 SNIFF_MASK 14
#define OFF   7

/*"Joystick" Init*/
#define UP     A1    /*Analog Pin 1*/
#define DOWN   A3    /*Analog Pin 3*/
#define LEFT   A5    /*Analog Pin 5*/
#define RIGHT  A2    /*Analog Pin 2*/
#define CLICK  A4    /*Analog Pin 4*/

// Clock defines
#define MINUTE 0
#define HOUR   1
#define ZERO   2

/*Status LED's*/
#define LED_2 8
#define LED_3 7

#define LCD_REQUEST         0x28F
#define LCD_1               0x290
#define LCD_1_INIT          0xC0
#define LCD_2               0x291
#define LCD_2_INIT          0x87

#define ENGINE_COOLANT_TEMP 0x05
#define ENGINE_RPM          0x0C
#define VEHICLE_SPEED       0x0D
#define IAT                 0x0F
#define MAF_SENSOR          0x10
#define THROTTLE            0x11
#define WB_AFR              0x34
#define OL_AFR              0x44
#define LOAD                0x43
#define AMBIENT_TEMP        0x46
#define CAT_TEMP            0x3C
#define STFT                0x06
#define LTFT                0x07
#define RUNTIME             0x1F
#define FUELLEVEL           0x2F
#define CYL_ADVANCE         0x0E
#define CONTROL_MODULE      0x42

#define PID_REQUEST         0x7DF
#define PID_REPLY			0x7E8

#define TPM_REQUEST         0x751
#define TPM_REPLY           0x759

#define CC_REQUEST          0x7E0 // Was 7E0 for some reason....
#define CC_REPLY            0x7E8
#define CC_UP				0xD6
#define CC_DOWN				0x9C
#define CC_CNCL				0x38

#define CC_ONOFF			0x648
#define CC_ONOFFRESP		0x650
#ifdef _DEBUGDATA
#define CC_POLLING          1
#else
#define CC_POLLING          100
#endif

#define LISTENONLY          9999

#define PARSEFAILURE       -9999

// RX8 Specific
static const unsigned short ID_650 = 0x650; // Cruise Control, data 0, x80 yellow, xff green
static const unsigned short ID_201 = 0x201; // RPM, data 0 and 1
static const unsigned short ID_231 = 0x231; // Gear selection
static const unsigned short ID_240 = 0x240; // Coolant, data 3
static const unsigned short ID_250 = 0x250; // IAT data 3
static const unsigned short ID_4B0 = 0x4B0; // Wheel Speed

#define HSCANCS     10   /* SPI Enable for HSCAN */
#define MSCANCS      9   /* SPI Enable for MSCAN */

#define HSCANINT     2
#define MSCANINT     3

#define RESETPIN 2
#define SCK 13 //spi
#define MISO 12
#define MOSI 11

//#define CANTIMEOUT 250

#endif
