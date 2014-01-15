/**
 ******************************************************************************
 * @file    simplevr_sample_command_test.ino
 * @author  Elechosue Team
 * @brief   This file provides a demostration on 
 * how to use SimpleVR to control led.
 ******************************************************************************
 * @note:
 *   Voice control led. Call name first, then call "turn on"/"turn off" command
 ******************************************************************************
 * @section  HISTORY
 * 
 * 2014/01/08    Initial version.
 */

#include <SoftwareSerial.h>
#include "SimpleVR.h"

/**        
 * Connection
 * Arduino    VoiceRecognitionModule
 * 2   ------->     TX
 * 3   ------->     RX
 */
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t buf[64];

void setup(void)
{
  myVR.begin(9600);
  
  /** initialize */
  Serial.begin(115200);
  Serial.println(F("Elechouse SimpleVR Module <Control LED> sample."));
  
  if(myVR.checkVersion(buf) > 0){
    Serial.println(F("Found SimpleVR"));
    Serial.print(F("SW VER:"));
    Serial.print(buf[0], DEC); 
    Serial.print('.');Serial.print(buf[1], DEC); 
    Serial.print('.');Serial.println(buf[2], DEC); 
    Serial.print(F("HW VER:"));
    Serial.print(buf[3], DEC); 
    Serial.print('.');Serial.println(buf[4], DEC); 
  }else{
    Serial.println(F("Can't find SimpleVR, please restart Arduino after checking the connection."));
    while(1);
  }
  
  if(myVR.setGroup(1) < 0){
    Serial.println(F("Communication error, please restart Arduino after checking the connection."));
    while(1);
  }
  
  if(myVR.setEnable(true) < 0){
    Serial.println(F("Communication error, please restart Arduino after checking the connection."));
    while(1);
  }
  
  if(myVR.checkSystemSettings(buf) < 0){
    Serial.println(F("Communication error, please restart Arduino after checking the connection."));
    while(1);
  }else{
    if(buf[0]){
      Serial.println(F("SimpleVR is enabled."));
    }else{
      Serial.println(F("SimpleVR is disabled."));
    }
    Serial.print(F("Group "));
    Serial.print(buf[1], DEC); Serial.println(F(" is selected."));
    if(buf[2] != 0xFF){
      Serial.print(F("Threshold value is ")); Serial.println(buf[2], DEC);
      Serial.println();
    }
  }
}

void loop(void)
{
  int ret;
  uint16_t voice=0;
  ret = myVR.recognize(buf, 50);
  if(ret > 0){
    voice += buf[0];
    voice <<=8;
    voice += buf[1];
    Serial.println(voice, DEC);
  }
}
