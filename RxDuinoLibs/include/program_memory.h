#ifndef _PROGRAM_MEMORY_
#define _PROGRAM_MEMORY_

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
 ************************************************************************/

// Initialize Mode Text 
const char* const allModesText[] PROGMEM =
  { ecuMonitorText, milesPerGalText, wheelSpeedText,   wheelPsiText,
    textModeText,   askClockText,    troubleCodesText, systemOffText,   
	respTimeText,   avgMpgText,      wheelTempText,    editTextText,    
	setClockText,   milClearText};
                
// Initialize Installed Monitors
const int allMonitors[] PROGMEM = 
   { ENGINE_RPM,   ENGINE_COOLANT_TEMP, VEHICLE_SPEED, MAF_SENSOR,    THROTTLE,
     OL_AFR,       WB_AFR,              LOAD,          CAT_TEMP,      IAT,
     STFT,         LTFT,                RUNTIME,       FUELLEVEL,     CONTROL_MODULE };  
                           
// Initialize Monitor Text                       
const char* const allMonitorsText[] PROGMEM =  
    { rpmLabel,   engTempLabel, speedLabel, mafLabel,  tpsLabel,  
      olafrLabel, wbafrLabel,   loadLabel,  catLabel,  iatLabel,  
      stftLabel,  ltftLabel,    runtLabel,  fuelLabel, controlLabel }; 
       
#endif
