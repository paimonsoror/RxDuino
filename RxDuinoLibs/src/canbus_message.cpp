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
 * Description: Functions that are used to handle all cruse control tasks
 *              for the RxDuino
 ************************************************************************/
#include <math.h>
#include "rxduino_defines.h"
#include "rxduino_methods.h"

#include "canbus_message.h"
#include "debug_methods.h"

/**
 * Perform HSCAN operations.  For Mazda's the HSCAN contains
 * main engine information.  Utilizing a 500kbps bus speed,
 * this method will control the HSCAN SPI toggles and send
 * and receive data
 *
 * @param  pid          The pid to scan from the ECU
 * @param  engineData   The response from the engine
 * @param  HSCAN		The HSCAN object
 * @param  byteOverride If set, used instead of 'byteCount'
 * @return              True if we were able to read data
 */
boolean CanbusMessage::getEngineData(const unsigned int& pid, String& engineData, 
									 MCP2515& HSCAN, const int& byteOverride) {
  RxduinoMethods::enableHSCAN();

  // CAN Structure
  Frame message;
  bool messageRead = false;
  boolean getEngineDataSuccess = false;
  
  // Byte count
  int bc = 0;
  if(byteOverride == 0)
	bc = getByteCount(pid);  
  else
    bc = byteOverride;
  
#ifndef _DEBUGDATA     
  if(pid != ID_4B0 && pid != LISTENONLY) {
     HSCAN.queryOBD(pid, message, messageRead);
  } else {
	 // Create the canbus message
	 createMessage(pid, message);
	 
     messageRead = HSCAN.receiveCANMessage(message, 25);
  }  
#else
   // Create the canbus message
   createMessage(pid, message);
   message.data[3] = random(0, 0x8C);
   message.data[4] = random(0, 0xA0);
   for(int i = 5; i < 8; i++) {
     message.data[i] = 0x00;
   } 
   messageRead = true;
#endif

  // Get The Response
  if(messageRead) {
    if(pid != LISTENONLY) {
	  // No need, dumping in the MCP lib
      if (pid != ID_4B0 && message.dataLength != byte(1)) {
        float value = parseMessage(message, bc);
		if(value != PARSEFAILURE)
			getEngineDataSuccess = getGenericMessage(message.data[2], value, engineData);
      } else {
        getEngineDataSuccess = getMazdaMessage(pid, message, engineData);
      }
    } else {
      // We are in sniffer mode
      DebugMethods::dumpMessage(message, "HSCAN"); 
    }
  }

  RxduinoMethods::disableCAN();
  
  return getEngineDataSuccess;
}

/** 
 * Get the byte count of the information based on the pid
 * @param pid	The pid that was requested
 * @return		The byte count
 */
int CanbusMessage::getByteCount(const unsigned int& pid) {	
	int byteCount = 0;
	switch(pid) {
		case ENGINE_COOLANT_TEMP:
		case VEHICLE_SPEED:
		case THROTTLE:
		case IAT:
		case STFT:
		case LTFT:
		case FUELLEVEL:
		byteCount = 1;
		break;
		
		case ENGINE_RPM:
		case MAF_SENSOR:
		case OL_AFR:
		case WB_AFR:
		case LOAD:
		case CAT_TEMP:
		case RUNTIME:
		case CONTROL_MODULE:
		byteCount = 2;
		break;
	}
	
	return byteCount;
}

/** 
 * Method to scan the data that is coming back from
 * the engine and convert it to human readable text
 *
 * @param  responsePid  The message response pid
 * @param  value        The value from the response
 * @param  engineData   The response from the engine
 */
boolean CanbusMessage::getGenericMessage(const byte& responsePid, const float& value, String& engineData) {
  float engine_data;
  boolean updated = false;
  String unit;
  int decimal = 0;
  
  // Some DEBUG methods
  DebugMethods::serialDebug("GetGenericMessage::responsePid %d\n", responsePid);
  DebugMethods::serialDebug("GetGenericMessage::value %d\n", value);
  
  switch(responsePid) {
    ////////////////////////////////////////////////////////////////////
    /// Two Byte Messages
    ////////////////////////////////////////////////////////////////////
    case ENGINE_RPM: 
    engine_data =  value/4;
	unit = "";
    updated = true;
    break;
    
    case MAF_SENSOR:
    engine_data =  value/100;
	unit = "G";
    updated = true;							
    break;
    
    case CONTROL_MODULE:
    engine_data = value/1000;
	unit = "v";
    updated = true;
    break;
    
    case WB_AFR:
    case OL_AFR:
    engine_data = (value/32768.0) * 14.7;
	decimal = 1;
    updated = true;
    break;

    case LOAD:
    engine_data = (value*100)/255;
	unit = "%";
    updated = true;
    break;

    case CAT_TEMP:
#ifndef _INTERNATIONAL	
    engine_data = ((value/10) - 40)*1.8;
    engine_data = engine_data + 32;
	unit = "*F";
#else
	engine_data = (value / 10) - 40;
	unit = "*C";
#endif
    updated = true;
    break;
        
    case RUNTIME:
    engine_data = value / 60;
	unit = "m";
    updated = true;
    break;
    
    ////////////////////////////////////////////////////////////////////
    /// One Byte Messages
    ////////////////////////////////////////////////////////////////////     
    case VEHICLE_SPEED: 
#ifndef _INTERNATIONAL
    engine_data =  (value / 1.609344);					
	unit = "mph";
#else
	engine_data = value;
	unit = "kph";
#endif
    updated = true;
    break;
        
    case IAT:
    case ENGINE_COOLANT_TEMP: 
    case AMBIENT_TEMP:
#ifndef _INTERNATIONAL	
    engine_data =  ((9/5) * (value - 40)) + 32; 
	unit = "*F";
#else
	engine_data = (value - 40);
	unit = "*C";
#endif
    updated = true;						
    break;

    case THROTTLE:
    engine_data = (value*100)/255;
	unit = "%";
    updated = true;
    break;
    
    case STFT:
    case LTFT:
    engine_data = ((value - 128) * 100/128);
	unit = "%";
    updated = true;
    break;
    
    case FUELLEVEL:
    engine_data = ((value * 100)/255);
	unit = "%";
    updated = true;
    break;
  }
  
  if(updated) {  
	char temp[10];
	dtostrf(engine_data, SCREEN2MAX - 1, decimal, temp);
	engineData = String(temp) + unit;
	engineData.trim();
	Serial.println(engineData);
  }	
  
  // return
  return updated;
}


/**
 * Get a mazda specific message
 *
 * @param    pid      The pid that we are looking for
 * @param    message  The message frame to analyze
 * @param    engineData The data from the engine
 * @return   True if successful reading
 */
boolean CanbusMessage::getMazdaMessage(const unsigned int& pid, const Frame& message, String& engineData) {
  boolean updated = false;
  char buffer[8];

  switch(message.adrsValue) {
    case ID_4B0:
    if(pid == ID_4B0) {
      int val[4], j = 0;
      for(int i = 0; i < 4; i++) {
        val[i] = (message.data[j++] << 0x08) + message.data[j++];
#ifndef _INTERNATIONAL
        val[i] = ((val[i] - 10000)/100) / 1.609344;
#else
		val[i] = (val[i] - 10000)/100;
#endif
      }
      sprintf(buffer, "%d/%d %d/%d", val[0], val[1], val[2], val[3]);
      updated = true;
    }
    break;
  }
  
  if(updated)
    engineData = String(buffer);  
	
  // return
  return updated;
}

//////////////////////////////////////////////////////////////////////////////////
// Canbus PID Parsing Function
//////////////////////////////////////////////////////////////////////////////////

/**
 * Parse the return message and store the data returned
 * depending on the number of bytes that the message contains
 *
 * @param message  The message to parse
 * @param bytes    The number of bytes in the message
 * @return		   The parsed message as a float
 */
float CanbusMessage::parseMessage(const Frame& message, const int& bytes) {
  DebugMethods::serialDebug("Parsing Message", true);
  float value = PARSEFAILURE;
  if(bytes == 1) {
    value = (float)message.data[3];
  } else if (bytes == 2) {
    value = ((float)message.data[3] * 256.0) + ((float)message.data[4]);
  }
  return value;  
}

/** 
 * Create a standard CANBUS message with the supplied PID
 * @param	pid	    The PID to request
 * @param	message The message object to modify
 */
void CanbusMessage::createMessage(const unsigned int& pid, Frame& message) {
  message.adrsValue = PID_REQUEST;
  message.isExtendedAdrs = false;
  message.extendedAdrsValue = 0;
  message.rtr = false;
  message.dataLength = 8;
  message.data[0] = 0x02;
  message.data[1] = 0x01;
  message.data[2] = pid;
  for(int i = 3; i < 8; i++) {
    message.data[i] = 0x00;
  }
}


//////////////////////////////////////////////////////////////////////////////////
// MPG Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * Estimate and display the current miles per gallon, or
 * display a running average of the current drive cycle
 * @param avg	Display running average if true
 */
bool CanbusMessage::getMilesPerGallon(String& engineData, float& avgMpg, const int& currentMode, MCP2515& HSCAN, MCP2515& MSCAN) {
  // The equation for MPG is
  // MPG = 710.7 * (VSS * 0.621371) / (MAF * 100)
  
  boolean avg = (currentMode == AVG_MPG);
  
  // First get our VSS
#ifdef _DEBUGDATA  
  if(true) {
	  String vspeed = "82mph";
#else	  
  if(CanbusMessage::getEngineData(VEHICLE_SPEED, engineData, HSCAN, 1)) { 
	  String vspeed = engineData;
#endif	  
	  delay(50);
	  
	  if(vspeed.length() > 3) {
		  // Remember vspeed ends with mph
		  vspeed = vspeed.substring(0, vspeed.length() - 3);
		  
		  // Now get our MAF
#ifdef _DEBUGDATA
		  if(true) {
			  String maf = "200G";
#else
		  if(CanbusMessage::getEngineData(MAF_SENSOR, engineData, HSCAN, 2)) {	  
			  String maf = engineData;
#endif				  
			  delay(50);
			  
			  if(maf.length() > 1) {
				  // Remember maf ends with G
				  maf = maf.substring(0, maf.length() - 1);
				  
				  char cVs[vspeed.length() + 1];
				  vspeed.toCharArray(cVs, vspeed.length() + 1);
				  int intSpeed = atoi(cVs);
				  
				  char cMaf[maf.length() + 1];
				  maf.toCharArray(cMaf, vspeed.length() + 1);
				  int intMaf   = atoi(cMaf);
				  
				  float mpg = 11.438 * (((float)intSpeed) / ((float)intMaf * 100.0));
				  
				  if(!isnan(mpg) && !isinf(mpg)) {
					  if(mpg > 0 && mpg < 50) {
						  if(!avg) {
							  char fstring[SCREEN2MAX - 1];
							  dtostrf(mpg, SCREEN2MAX - 1, 1, fstring);
							  //sprintf(buffer, "Mi/Gal:%s", fstring);
							  engineData = "Mi/Gal:" + String(fstring);
						  } else {
							  avgMpg = (avgMpg + mpg)/2;
							  char fstring[SCREEN2MAX - 1];
							  dtostrf(avgMpg, SCREEN2MAX - 1, 2, fstring);
							  //sprintf(buffer, "AvgMPG:%s", fstring);
							  engineData = "AvgMPG:" + String(fstring);
						  }
						  //LCDMethods::setLcdMessage(buffer, MSCAN, currentMode);
						  //memset(buffer, 0, sizeof(buffer));
					  }					  
				  } else {
					  engineData = "Mi/Gal:--";
				  }
				  
				  // Cleanup
				  memset(cVs, 0, sizeof(cVs));
				  memset(cMaf, 0, sizeof(cMaf));
			  }
		  }
	  }
  }
  
  return true;
}