#include <SoftwareSerial.h>
//===============NUMBER OF MACHINE===========//
#define RECIVEMACHINE  01
#define PAYBILLMACHINE 02
#define PAYCOINMACHINE 03
//===============ERROR OF MACHINE===========//
#define NOERROR		0xFE
#define TIMEOUT		0xFD


byte SenddataToRasberry[7] = {0xFF,0xFF,0x04,0x00,0x00,0x00,0x00}; //===(start1,start2,length,machine,error.status,value,checksum)===//
byte NumMachine;
byte Function;


//==============Packet Calc===============

#define PRO_START0			_indexStartReader
#define PRO_START1			_indexStartReader+1
#define PRO_LENGTH			_indexStartReader+2
#define PRO_ID                          _indexStartReader+3
#define PRO_FUNCTION                    _indexStartReader+4
#define PRO_VALUE                       _indexStartReader+5
#define PRO_CHECKSUM		        _indexStartReader+2+rxBuffer[PRO_LENGTH]

#define INC_RING(var, bsize) if (var == ((bsize)-1)) var=0; else var++
#define CLEAR_BUFFER rxWritePointer = rxReadPointer  = 0

#define RX_BUFFER_SIZE 200
byte rxBuffer[RX_BUFFER_SIZE];
byte rxWritePointer = 0;
byte rxReadPointer = 0;
byte state = 0;
byte _indexStartReader;

byte _rxState = 0;
byte _byte_count;
byte _checksum;

//========================================

//=================Get status ==================== 
unsigned long previousMillisReciveBill = 0;  
unsigned long previousMillisPayBill = 0; 
unsigned long previousMillisPayCoin = 0; 

const long interval = 180000;// time want check status 180 sec
boolean flagCheckStatus = false;
#define RESETTIMECHECKSTATUS previousMillisReciveBill = previousMillisPayBill=  previousMillisPayCoin  = millis()
//=================================================

void setup()
{
  InitBillRecive();
  InitBillPay();
  InitCoinPay();
  Serial.begin(9600);
  
}

void loop() 
{
  Rx_Calc();//Calc packet from raspberry pi
  LoopCheckStatus();//Check status   
  
  CalcRxDataBillRecive();
  
}


#define P_RECIVEBILL  0x01

#define F_RECIVEBILL  0x01
#define F_CHECKSTATUS 0x02
#define F_RESET       0x03
#define F_ACCEPT      0x04

#define P_PAYBILL     0x02
#define F_PAYBILL     0x01

#define P_PAYCOIN     0x03
#define F_PAYCOIN     0x03
void CalcCommandFromRaspberryPi()
{
 
  RESETTIMECHECKSTATUS;
  
  byte _id = rxBuffer[PRO_ID];
  byte _function = rxBuffer[PRO_FUNCTION]; 
  byte _value = rxBuffer[PRO_VALUE];
//   Serial.println("val");
//    Serial.write(_id);
//    Serial.write(_function);
//    Serial.write(_value);
  switch(_id)
  {
	  
      case P_RECIVEBILL:
              //Serial.println("P_RECIVEBILL");
              switch(_function)
              {
                case F_RECIVEBILL:ReciveBill();break;
                //case F_CHECKSTATUS: CheckStatusReciveBill();break;
                case F_RESET: ResetMachineReciveBill();break;
                case F_ACCEPT:SendDataToMachine(0x02);break;

              }
           break;
           
      case P_PAYBILL:
              switch(_function)
              {
                case F_RECIVEBILL: Paybill(_value);break;
                //case F_CHECKSTATUS: CheckStatusPayBill();break;
                //case F_RESET: break;
              }
           break;
      
      case P_PAYCOIN:
              switch(_function)
              {
                case F_PAYCOIN: PayCoin(_value);break;
                //case F_CHECKSTATUS: CheckStatusCoin();break;
                //case F_RESET: break;
              }
      break; 
  }
  
}

//@Overwrite Interrup
void serialEvent() {
  while (Serial.available()) 
  {
    // get the new byte:
    rxBuffer[rxWritePointer] = (char)Serial.read(); 
    INC_RING(rxWritePointer,RX_BUFFER_SIZE);
    //Serial.println("get");
  }
}

//Calc buffer income data
void Rx_Calc()
{
    if(rxReadPointer != rxWritePointer)
    {
		
    }else{
	return;
    }

    byte _UDR = rxBuffer[rxReadPointer];
    INC_RING(rxReadPointer,RX_BUFFER_SIZE);
   
    switch(state)
	{
		case 0://star1
		if (_UDR == 0xFF)state =1;break;
		case 1://star2
		if (_UDR == 0xFF)
		{
			state =2;
			_indexStartReader = rxReadPointer-2;
		}
		else
		{
			state = 0;
		}
		break;
		
		case 2://LENGTH
		_byte_count=_UDR;state =3;if(_UDR > 20)state =0;break;

		case 3://DATA
		_byte_count--;
		if (_byte_count == 1) state =4;
		
		break;
		
		case 4:
		
		if(checkSum(PRO_LENGTH,PRO_CHECKSUM)== _UDR)
		{
			//Serial.println("check sum ok!");
// 			Serial.write(checkSum(PRO_LENGTH,PRO_CHECKSUM));
// 			Serial.write(_UDR);			
                     CalcCommandFromRaspberryPi();

		}else
		{
			
			//check sum error
// 			Serial.write(checkSum(PRO_LENGTH,PRO_CHECKSUM));
// 			Serial.write(_UDR);
// 			for (int i =0;i<7;i++)
// 			{
// 				Serial.write(rxBuffer[i]);
// 			}
			//Serial.println("check sum error");
		}
		state = 0;
		break;
	} 
    //Debug
    //Serial.print(_UDR,HEX);
    //Serial.print(" state = ");
    //Serial.print(state,HEX);
    //Serial.print(" bytecount= ");
    //Serial.print(_byte_count,HEX);
    //Serial.print("\r\n");
}
byte checkSum(byte _start,byte _stop)
{
	byte check_Sum = 0;
	for(;_start != _stop;  _start++)
	{
		check_Sum  += rxBuffer[_start];
	}
	return ~check_Sum;
}


void SendToRasberry(byte *data,byte lengthR)
{
	for (int i=0;i<lengthR;i++)
	{
		Serial.write((char)(data[i]));
	}	
}


void LoopCheckStatus()
{
	 unsigned long currentMillis = millis();	 
	 if(currentMillis - previousMillisReciveBill >= interval) {
		 previousMillisReciveBill = currentMillis;
		 // ===========make check status ReciveBill ==================== 
		 byte statusRecive =  CheckStatusReciveBill();
		 if (statusRecive!=NOERROR)
		 {
			 
		 }
	 }

         if(currentMillis - previousMillisPayBill >= interval + 2000) {
		 previousMillisPayBill = currentMillis;
		 // ===========make check status PayBill ==================== 
		 
	 }

         if(currentMillis - previousMillisPayCoin >= interval + 4000) {
		 previousMillisPayCoin = currentMillis;
		 // ===========make check status PayCoin ==================== 

	 }


}

