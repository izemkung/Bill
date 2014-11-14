/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
void CalcCommandFromRaspberryPi();
void serialEvent();
void Rx_Calc();
byte checkSum(byte _start,byte _stop);
void SendToRasberry(byte *data,byte lengthR);
void LoopCheckStatus();
void InitBillPay(void);
void SendDataPayBill(byte *data,byte lengthb);
void Paybill(int num);
void CheckStatusPayBill();
int8_t WaitCommandBill(byte *expected_answer,byte l ,unsigned int timeout);
void ResetPayBill();
byte CheckStatus(byte data);
void PacketToRasberryPayBill(byte status,byte lengthR);
void InitCoinPay(void);
void SendDataCoin(byte *data,byte lengths);
void PayCoin(int num);
void SendInquire();
void ResetPayCoin();
int8_t WaitCommandCoin(byte *expected_answer,byte l ,unsigned int timeout);
void CheckStatusCoin();
void PacketToRasberryPayCoin(byte status,byte _value,byte lengthR);
void InitBillRecive(void);
void ReciveBill(void);
void SendDataToMachine(byte data);
void SendDataToRassberry01(byte *data,byte lengthR);
void CalcRxData();
int8_t WaitCommand(byte *expected_answer, unsigned int timeout);
boolean ByteArrayCompare(byte a[],byte b[],int array_size);
byte CheckStatusReciveBill();
byte CheckValueBill(byte data);
void EnableReciveBill();
void DisnableReciveBill();
void ResetMachineReciveBill();
void PacketToRasberryReciveBill(byte status,byte lengthR);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\Nrp\Documents\GitHub\project\Bill\Arduino\MainProject\MainProject.ino"
#include "C:\Users\Nrp\Documents\GitHub\project\Bill\Arduino\MainProject\PayBill.ino"
#include "C:\Users\Nrp\Documents\GitHub\project\Bill\Arduino\MainProject\PayCoin.ino"
#include "C:\Users\Nrp\Documents\GitHub\project\Bill\Arduino\MainProject\ReciveBill.ino"
