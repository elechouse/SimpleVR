/**
  ******************************************************************************
  * @file    SimpleVR.h
  * @author  Elechouse Team
  * @version V1.0
  * @date    2014-1-8
  * @brief   This file provides all the SimpleVR firmware functions.
  ******************************************************************************
    @note
         This driver is for elechouse SimpleVR Module(LINKS here)
  ******************************************************************************
  * @section  HISTORY
  
    V1.0    Initial version.
  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ELECHOUSE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 ELECHOUSE</center></h2>
  ******************************************************************************
  */
  
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "wiring_private.h"

#include "SoftwareSerial.h"
#include <avr/pgmspace.h>

#define DEBUG

#ifdef DEBUG
#define DBGSTR(message)     Serial.print(message)
#define DBGBUF(buf, len)	Serial.write(buf, len)
#define DBGLN(message)		Serial.println(message)
#define DBGFMT(msg, fmt)	Serial.print(msg, fmt)
#define DBGCHAR(c)			Serial.write(c)
#else
#define DBG(message)
#endif // DEBUG

#define VR_DEFAULT_TIMEOUT					(1000)

/***************************************************************************/
#define FRAME_HEAD							(0xAA)
#define FRAME_END							(0x0A)

/***************************************************************************/
#define FRAME_CMD_SYS_RESET 				(0x00)
#define FRAME_CMD_ENABLE_VR					(0x01)
#define FRAME_CMD_DISABLE_VR				(0x02)
#define FRAME_CMD_GRP						(0x03)
#define FRAME_CMD_SYS_INFO_CTL				(0x04)
#define FRAME_CMD_THRESHOLD					(0x05)

#define FRAME_CMD_CHECK_SYS					(0x10)	//
#define FRAME_CMD_CHECK_VERSION				(0x11)	//

#define FRAME_CMD_VR						(0x0D)	//Voice recognized
#define FRAME_CMD_PROMPT					(0x0A)
#define FRAME_CMD_ERROR						(0xFF)

/***************************************************************************/
// #define FRAME_ERR_UDCMD						(0x00)
// #define FRAME_ERR_LEN						(0x01)
// #define FRAME_ERR_DATA						(0x02)
// #define FRAME_ERR_SUBCMD					(0x03)

// //#define FRAME_ERR_
// #define FRAME_STA_SUCCESS					(0x00)
// #define FRAME_STA_FAILED					(0xFF)
/***************************************************************************/


class VR : public SoftwareSerial{
public:
	VR(uint8_t receivePin, uint8_t transmitPin);
	
	static VR* getInstance() {
	   return instance;
	}

	int restoreSystemSettings();

    int checkSystemSettings(uint8_t* buf);
    int checkVersion(uint8_t* buf);
    
    int recognize(uint8_t *buf, int timeout = VR_DEFAULT_TIMEOUT);

    int setGroup(uint8_t grp);
    int setThreshold(uint8_t val);

    int setEnable(bool sta=true);
    int setDisable(bool sta=true);

    int setStartInfoEnable(uint8_t sta=true);
    int setStartInfoDisable(uint8_t sta=true);
	
	int writehex(uint8_t *buf, uint8_t len);
	
/***************************************************************************/
	/** low level */
	int len(uint8_t *buf);
	int cmp(uint8_t *buf, uint8_t *bufcmp, int len  );
	void cpy(char *buf,  char * pbuf);
	void sort(uint8_t *buf, int len);
	int cleanDup(uint8_t *des, uint8_t *buf, int len);
	void send_pkt(uint8_t *buf, uint8_t len);
	void send_pkt(uint8_t cmd, uint8_t *buf, uint8_t len);
	void send_pkt(uint8_t cmd, uint8_t subcmd, uint8_t *buf, uint8_t len);
	int receive(uint8_t *buf, int len, uint16_t timeout = VR_DEFAULT_TIMEOUT);
	int receive_pkt(uint8_t *buf, uint16_t timeout = VR_DEFAULT_TIMEOUT);
/***************************************************************************/
private:
	static VR*  instance;
};

