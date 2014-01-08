/**
  ******************************************************************************
  * @file    SimpleVR.cpp
  * @author  Elechouse Team
  * @version V1.0
  * @date    2014-01-08
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
#include "SimpleVR.h"
#include <string.h>

VR* VR::instance;

/** temp data buffer */
uint8_t vr_buf[32];
uint8_t hextab[17]="0123456789ABCDEF";

/**
	@brief VR class constructor.
	@param receivePin --> software serial RX
		   transmitPin --> software serial TX
*/
VR::VR(uint8_t receivePin, uint8_t transmitPin) : SoftwareSerial(receivePin, transmitPin)
{
	instance = this;
	SoftwareSerial::begin(38400);
}

/**
	@brief read recognise result
	@param buf --> return data .
			 buf[0]  -->  high byte of sentence index value
             buf[1]  -->  low byte of sentence index value
             buf[2]  -->  sentences group number
             buf[3]  -->  score of recognition.
		   timeout --> wait time for receiving packet.
	@retval length of valid data in buf. 0 means no data received.
*/
int VR :: recognize(uint8_t *buf, int timeout)
{
	int ret, i;
	ret = receive_pkt(vr_buf, timeout);
	if(vr_buf[2] != FRAME_CMD_VR){
		return -1;
	}
	if(ret > 0){
		for(i = 0; i < (vr_buf[1] - 3); i++){
			buf[i] = vr_buf[4+i];
		} 
		return i;
	}
	
	return 0;
}

/**
    @brief reset system setting to default
    @retval  0 --> success
            -1 --> failed
*/
int VR :: restoreSystemSettings()
{
	int len;
	send_pkt(FRAME_CMD_SYS_RESET, 0, 0);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}

	if(vr_buf[2] != FRAME_CMD_SYS_RESET){
		return -1;
	}

	return 0;
}

/**
    @brief check system settings
    @param buf --> return value
            buf[0] --> work state
            buf[1] --> group selected
            buf[2] --> score
    @retval '>0' --> buf length
            -1 --> failed
*/
int VR :: checkSystemSettings(uint8_t* buf)
{
	int len;
	if(buf == 0){
		return -1;
	}
	send_pkt(FRAME_CMD_CHECK_SYS, 0, 0);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != FRAME_CMD_CHECK_SYS){
		return -1;
	}

	memcpy(buf, vr_buf+4, vr_buf[1]-3);
	return vr_buf[1]-3;
}

/**
    @brief check simplevr version
    @param buf --> return value
            buf[0] --> software version major
            buf[1] --> software version minor
            buf[2] --> software version patch
            buf[3] --> hardware version major
            buf[4] --> hardware version minor
    @retval '>0' --> buf length
            -1 --> failed
*/
int VR :: checkVersion(uint8_t* buf)
{
	int len;
	if(buf == 0){
		return -1;
	}
	send_pkt(FRAME_CMD_CHECK_VERSION, 0, 0);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != FRAME_CMD_CHECK_VERSION){
		return -1;
	}

	memcpy(buf, vr_buf+4, vr_buf[1]-3);
	return vr_buf[1]-3;
}

/**
    @brief select and enable a group
    @param grp --> group number
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setGroup(uint8_t grp)
{
	int len;
	if(grp == 0 || grp > 64){
		return -1;
	}
	send_pkt(FRAME_CMD_GRP, &grp, 1);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != FRAME_CMD_GRP){
		return -1;
	}

	return 0;
}

/**
    @brief set threshold value
    @param val --> threshold value
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setThreshold(uint8_t val)
{
	int len;
	send_pkt(FRAME_CMD_THRESHOLD, &val, 1);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != FRAME_CMD_THRESHOLD){
		return -1;
	}

	return 0;
}

/**
    @brief enable SimpleVR
    @param sta -->  new state
    			true  --> enable
    			false --> disable
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setEnable(bool sta)
{
	int len;
	uint8_t cmd;
	if(sta){
		cmd = FRAME_CMD_ENABLE_VR;
	}else{
		cmd = FRAME_CMD_DISABLE_VR;
	}
	send_pkt((uint8_t)cmd, 0, 0);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != cmd){
		return -1;
	}

	return 0;
}

/**
    @brief disable SimpleVR
    @param sta -->  new state
    			true  --> disable
    			false --> enable
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setDisable(bool sta)
{
	return setEnable(!sta);
}

/**
    @brief enable startup information
    @param sta -->  new state
    			true  --> enalbe
    			false --> disable
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setStartInfoEnable(uint8_t sta)
{
	int len;
	uint8_t cmd;
	if(sta){
		cmd = 1;
	}else{
		cmd = 0;
	}
	send_pkt(FRAME_CMD_SYS_INFO_CTL, &cmd, 1);
	len = receive_pkt(vr_buf);
	if(len<=0){
		return -1;
	}
	
	if(vr_buf[2] != cmd){
		return -1;
	}

	return 0;
}

/**
    @brief disable startup information
    @param sta -->  new state
    			true  --> disable
    			false --> enable
    @retval  0 --> success
            -1 --> failed
*/
int VR :: setStartInfoDisable(uint8_t sta)
{
	return setStartInfoEnable(!sta);
}

/**flash operation function (strlen)*/
int VR :: len(uint8_t *buf)
{
	int i=0;
	while(pgm_read_byte_near(buf++)){
		i++;
	}
	return i;
}

/**flash operation function (strcmp)*/
int VR :: cmp(uint8_t *buf, uint8_t *bufcmp, int len  )
{
	int i;
	for(i=0; i<len; i++){
		if(buf[i] != pgm_read_byte_near(bufcmp+i)){
			return -1;
		}
	}
	return 0;
}

/**flash operation function (strcpy)*/
void VR :: cpy(char *buf, char * pbuf)
{
  int i=0;
  while(pgm_read_byte_near(pbuf)){
    buf[i] = pgm_read_byte_near(pbuf++);
    i++;
  }
}

/** ascending sort */
void VR :: sort(uint8_t *buf, int len)
{
	int i, j;
	uint8_t tmp;
	for(i=0; i<len; i++){
		for(j=i+1; j<len; j++){
			if(buf[j] < buf[i]){
				tmp = buf[i];
				buf[i] = buf[j]; 
				buf[j] = tmp;
			}
		}
	}
}

/** remove duplicates */
int VR :: cleanDup(uint8_t *des, uint8_t *buf, int len)
{	
	if(len<1){
		return -1;
	}
	
	int i, j, k=0;
	for(i=0; i<len; i++){
		for(j=0; j<k; j++){
			if(buf[i] == des[j]){
				break;
			}
		}
		if(j==k){
			des[k] = buf[i];
			k++;
		}
	}
	return k;
#if 0
	int i=0, j=1, k=0;
	sort(buf, len);
	for(; j<len; ){
		des[k] = buf[i];
		k++;
		while(buf[i]==buf[j]){
			j++;
			if(j==len){
				return k;
			}
		}
		i=j;
	}
	return k;
#endif
}

/** Serial print in HEX format */
int VR :: writehex(uint8_t *buf, uint8_t len)
{
	int i;
	for(i=0; i<len; i++){
		DBGCHAR(hextab[(buf[i]&0xF0)>>4]);
		DBGCHAR(hextab[(buf[i]&0x0F)]);
		DBGCHAR(' ');
	}
	return len;
}

/**
    @brief send data packet in Voice Recognition module protocol format.
    @param cmd --> command 
           subcmd --> subcommand
           buf --> data area
           len --> length of buf
*/
void VR :: send_pkt(uint8_t cmd, uint8_t subcmd, uint8_t *buf, uint8_t len)
{
	while(available()){
		flush();
	}
	write(FRAME_HEAD);
	write(len+3);
	write(cmd);
	write(subcmd);
	write(buf, len);
	write(FRAME_END);
}

/**
    @brief send data packet in Voice Recognition module protocol format.
    @param cmd --> command 
           buf --> data area
           len --> length of buf
*/
void VR :: send_pkt(uint8_t cmd, uint8_t *buf, uint8_t len)
{
	while(available()){
		flush();
	}
	write(FRAME_HEAD);
	write(len+2);
	write(cmd);
	write(buf, len);
	write(FRAME_END);
}

/**
    @brief send data packet in Voice Recognition module protocol format.
    @param buf --> data area
           len --> length of buf
*/
void VR :: send_pkt(uint8_t *buf, uint8_t len)
{
	while(available()){
		flush();
	}
	write(FRAME_HEAD);
	write(len+1);
	write(buf, len);
	write(FRAME_END);
}

/**
    @brief receive a valid data packet in Voice Recognition module protocol format.
    @param buf --> return value buffer.
           timeout --> time of reveiving
    @retval '>0' --> success, packet lenght(length of all data in buf)
            '<0' --> failed
*/
int VR :: receive_pkt(uint8_t *buf, uint16_t timeout)
{
	int ret;
	ret = receive(buf, 2, timeout);
	if(ret != 2){
		return -1;
	}
	if(buf[0] != FRAME_HEAD){
		return -2;
	}
	if(buf[1] < 2){
		return -3;
	}
	ret = receive(buf+2, buf[1], timeout);
	if(buf[buf[1]+1] != FRAME_END){
		return -4;
	}
	
//	DBGBUF(buf, buf[1]+2);
	
	return buf[1]+2;
}

/**
    @brief receive data .
    @param buf --> return value buffer.
           len --> length expect to receive.
           timeout --> time of reveiving
    @retval number of received bytes, 0 means no data received.
*/
int VR::receive(uint8_t *buf, int len, uint16_t timeout)
{
  int read_bytes = 0;
  int ret;
  unsigned long start_millis;
  
  while (read_bytes < len) {
    start_millis = millis();
    do {
      ret = read();
      if (ret >= 0) {
#if 0
  		uint8_t tmp = ret;
      	writehex(&tmp, 1);
#endif        
        break;
     }
    } while( (millis()- start_millis ) < timeout);
    
    if (ret < 0) {
      return read_bytes;
    }
    buf[read_bytes] = (char)ret;
    read_bytes++;
  }
  
  return read_bytes;
}
