//===============NUMBER OF MACHINE===========//
#define RECIVEMACHINE  01
#define PAYBILLMACHINE 02
#define PAYCOINMACHINE 03
//===============ERROR OF MACHINE===========//
#define NOERROR		01
#define TIMEOUT		02


byte SenddataToRasberry[7] = {0xFF,0xFF,0x04,0x00,0x00,0x00,0x00}; //===(start1,start2,length,machine,error.status,value,checksum)===//
byte ReadFromRasberry[7];
byte NumMachine;
byte Function;


//==============Packet Calc===============

#define PRO_START0			_indexStartReader
#define PRO_START1			_indexStartReader+1
#define PRO_LENGTH			_indexStartReader+2
#define PRO_ID                          _indexStartReader+3
#define PRO_ERROR                       _indexStartReader+4
#define PRO_VALUE                       _indexStartReader+5
#define PRO_CHECKSUM		        _indexStartReader+2+rxBuffer[PRO_LENGTH]

#define INC_RING(var, bsize) if (var == ((bsize)-1)) var=0; else var++
#define CLEAR_BUFFER rxWritePointer = rxReadPointer  = 0

#define RX_BUFFER_SIZE 200
byte rxBuffer[RX_BUFFER_SIZE];
byte rxWritePointer = 0;
byte rxReadPointer = 0;

byte _indexStartReader;

byte _rxState = 0;
byte _byte_count;
byte _id;
byte _checksum;
//========================================

unsigned long previousMillis = 0;        
const long interval = 2000; //=================loop time want check status=============================//
boolean flagCheckStatus = false;


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
}

void CalcCommandFromRaspberryPi()
{
  byte _id = rxBuffer[PRO_ID];
  byte _function = rxBuffer[PRO_ID]; 
  byte _value = rxBuffer[PRO_ID];
  
  switch(_id)
  {
      case 1:
      
                break;
      case 2:break; 
      case 3:break; 
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
		        //CalcParameter();
                        CalcCommandFromRaspberryPi();


                        byte _id = rxBuffer[PRO_ID] -1;
                        if(_id >= 0 && _id <= LEDNUM)
                        {
			  previousMillis[_id] = 0;
                          LedBuffer[_id][A_R] = rxBuffer[PRO_R];
                          LedBuffer[_id][A_G] = rxBuffer[PRO_G];
                          LedBuffer[_id][A_B] = rxBuffer[PRO_B];
                          LedBuffer[_id][A_MODE] = rxBuffer[PRO_MODE];
                          LedBuffer[_id][A_TIME] = rxBuffer[PRO_TIME];
                          LedBuffer[_id][A_TIME2] = rxBuffer[PRO_TIME2];
                        }else if(_id == 253)//id - 1
                        {
                          for(int i = 0;i<=LEDNUM+1;i++)
                          {
                            previousMillis[i] = 0;
                            LedBuffer[i][A_R] = rxBuffer[PRO_R];
                            LedBuffer[i][A_G] = rxBuffer[PRO_G];
                            LedBuffer[i][A_B] = rxBuffer[PRO_B];
                            LedBuffer[i][A_MODE] = rxBuffer[PRO_MODE];
                            LedBuffer[i][A_TIME] = rxBuffer[PRO_TIME];
                            LedBuffer[i][A_TIME2] = rxBuffer[PRO_TIME2];
                          }
                        }

		}else
		{
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
	 if(flagCheckStatus==true)return; //Ennable Disable function
	 unsigned long currentMillis = millis();	 
	 if(currentMillis - previousMillis >= interval) {
		 // save the last time you blinked the LED
		 previousMillis = currentMillis;
		 // ============================make check status====================================
		 
	 }
}

