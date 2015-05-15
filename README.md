# SimpleVR

Arduino Library For [Elechouse SimpleVR](http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2263)

## Feature
- 2 LEDs are used to indicate the status of the module. *LED above* indicates that whether SimpleVR is enabled, 'on' means 'enable', 'off' means 'disable'; *LED below* blinks once when data is sent or received.
- 1 uart port with fixed uart baud rate 9600
- 1 USB port, used to upgrade firmware and configure the ASR command.
- Support maximum 64 groups, each group can contain maximum 2000 sentences.
- Command format is compatible with [Elechouse Voice Recognition V3](https://github.com/elechouse/VoiceRecognitionV3)

## Quick Start

### Prepare
+ [SimpleVR module][SimpleVR] 
+ [Arduino board][Arduino]  ([UNO][UNO] recommended)
+ [Arduino IDE][ArduinoIDE]
+ SimpleVR library([Download zip file][dzip])

### How To
1. Connect your SimpleVR Module with Arduino.
1. Download SimpleVR library, open example simplevr_sample_control_led.
1. Upload to Arduino.
1. Call name to enable control command voice, call turn on or turn off command voice to turn led on/off.

***See SimpleVR client instruction to know how to update voice command.***

## Protocol
The simplest way to play the Voice Recognition V3 module is to use this VoiceRecognition Arduino library. But for many **hackers**, this is far from enough, so we supply this protocol by which user can communicate with the Voice Recognition V3 module directly.

### Base Format

#### Control
**| Head (0AAH) | Length| Command | Data | End (0AH) |**  
Length = L(Length + Command + Data)

#### Return
**| Head (0AAH) | Length| Command | Data | End (0AH) |**  
Length = L(Length + Command + Data)

NOTE: Same data area of different commands has different meanings.

### Codes
[index]: #codes
***ALL CODES ARE IN HEXADECIMAL FORMAT***

---  
***FRAME CODE***  
**AA** --> Frame Head  
**0A** --> Frame End  

---
***SET***  
**00** --> [Restore system settings][id00]  
**01** --> [Enable recognition][id01]  
**02** --> [Disable recognition][id02]  
**03** --> [Group select][id03]  
**04** --> [Enable/Disable startup information][id04]  
**05** --> [Set threshold value][id05]  

---
***CHECK***  
**10** --> [Check System State][id10]  
**11** --> [Check Version Info][id11]  

---
***THESE 3 COMMANDS ARE ONLY USED FOR RETURN MESSAGE***  
**0A** --> [Prompt][id0a]  
**0D** --> [Voice Recognized][id0d]  
**FF** --> [Error][idff]  

### Details

[id00]: #restore-system-settings-00
#### Restore System Settings (00)
Use "Restore System Settings" command to reset SimpleVR to default(Enabled/Startup Info/Group 1).  
**Format:**  
| AA | 02 | 00 | 0A |  
**Return:**  
| AA | 03 | 00 | 00 | 0A |  
***If an error occurs, error info is returned.***

[Back to index][index]
[id01]: #enable-recognition-01
#### Enable recognition (01)
Use to enable recognition(or said enable SimpleVR), LED above would be turned on.  
**Format:**  
| AA | 02 | 01 | 0A |  
**Return:**  
| AA | 03 | 01 | 00 | 0A |  
***If an error occurs, error info is returned.***

[Back to index][index]
[id02]: #disable-recognition-02
#### Disable recognition (02)
Use to disable recognition(or says disable SimpleVR), LED above would be turned off.  
**Format:**  
| AA | 02 | 02 | 0A |  
**Return:**  
| AA | 03 | 02 | 00 | 0A |  
***If an error occurs, error info is returned.***

[Back to index][index]
[id03]: #Group-Select-03
#### Group Select (03)
Use to select and enable a group.  
**Format:**  
| AA | 03 | 03 | GRP| 0A |  
**Return:**  
| AA | 03 | 03 | 00 | 0A |  
***If an error occurs, error info is returned.***  
**GRP**: Group number to select, range from 1 to 64.  

[Back to index][index]
[id04]: #disable-or-enable-startup-info-04
#### Disable or enable startup info (04)
Use to disable/enable the startup info.  
**Format:**  
| AA | 03 | 04 | STA | 0A |  
**Return:**  
| AA | 03 | 04 | 00 | 0A |  
***If an error occurs, error info is returned.***  
**STA**: 0 -- diable, others -- enable.

[Back to index][index]
[id05]: #set-threshold-value-05
#### Set Threshold Value (05)
Use to disable/enable the startup info.  
**Format:**  
| AA | 03 | 05 | VAL | 0A |  
**Return:**  
| AA | 03 | 05 | 00 | 0A |  
***If an error occurs, error info is returned.***  
**VAL**: Threshold value, recommend value is (0x45-0x65). 

[Back to index][index]
[id10]: #check-system-state-10
#### Check System State (10)
Use to disable/enable the startup info.  
**Format:**  
| AA | 02 | 10 | 0A |  
**Return:**  
| AA | 06 | 10 | 00 | STA | GRP | VAL | 0A |  
***If an error occurs, error info is returned.***  
**STA**: Work State (00 -- disabled, others -- enabled)  
**GRP**: Current group  
**VAL**: Current threshold value.

[Back to index][index]
[id11]: #check-version-11
#### Check Version (11)
Use to disable/enable the startup info.  
**Format:**  
| AA | 02 | 11 | 0A |  
**Return:**  
| AA | 08 | 11 | 00 | SWMAJOR | SWMINOR | SWPATCH | HWMAJOR | HWMINOR | 0A |  
***If an error occurs, error info is returned.***  
Software version: **MAJOR.MINOR.PATCH**  
Hardware version: **MAJOR.MINOR**

[Back to index][index]
[id0d]: #voice-recognized-0d
#### Voice Recognized (0D)
**Voice Recognized** command is only used for Voice Recognition Module to return data when voice is recognized.  
**Format:**  
NONE  
**Return:**  
| AA | 07 | 0D | 00 | INDEXH | INDEXL | GRP | SCORE | 0A |  
**INDEXH**: High byte of sentence index value.  
**INDEXL**: Low byte of sentence index value.  
**GRP**: Current group.  
**SCORE**: Score of recognition. The higher the score to recognize the more accurate.

[Back to index][index]
[idff]: #error-ff
#### Error (FF)
Error command is only used for Voice Recognition Module to return error status.  
**Format:**  
NONE  
**Return:**  
| AA | 03 | FF | ECODE | 0A |  

**ECODE**: error code (FF-command undefined FE-command length error FD-data error FC-subcommand error FB-command usage error)

[Back to index][index]
## Buy
[![elechouse][EHICON]][EHLINK]

[EHLINK]: http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2263

[EHICON]: https://raw.github.com/elechouse/CarDriverShield/master/image/elechouse.png

[accessport]: http://www.sudt.com/en/ap/       "AccessPort"

[ArduinoIDE]: http://arduino.cc/en/main/software "Arduino IDE"

[UNO]: http://arduino.cc/en/Main/arduinoBoardUno

[SimpleVR]: http://www.elechouse.com

[Arduino]: http://arduino.cc/en/

[dzip]: https://github.com/elechouse/SimpleVR/archive/master.zip

[EHLINK]: http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2263

[EHICON]: https://raw.github.com/elechouse/CarDriverShield/master/image/elechouse.png


[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/elechouse/simplevr/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

