
//===============NUMBER OF MACHINE===========//
#define RECIVEMACHINE  01
#define PAYBILLMACHINE 02
#define PAYCOINMACHINE 03

//==========Status Coin========================//
#define STATUS_NO_ERROR            0x00
#define STATUS_LOWCOIN             0x20
#define STATUS_NOCOIN_NOTFINISH    0x21
#define STATUS_EMPTYCOIN           0x28
#define STATUS_PAYCOIN_ERROR       0x29
#define STATUS_ERROR			   0x01

#define NOERROR		0xFE
#define TIMEOUT		0xFD
#define OK			0xFC
#define EMPTY		0xFB
#define Coin Serial2
//===================Command Control============//
byte DataToRasberry03[7] = {0xFF,0xFF,0x04,0x03,0x00,0x00,0x00}; //===(start1,start2,length,machine,error.status,value,checksum)===//
byte CheckSumToRasberry03;
byte errorMachinePayCoin;
byte dataerror[2];
byte ResetC[8] = {0xED,0x08,0x00,0x50,0x00,0x00,0x00,0xB2};
byte Inquire[8] = {0xED,0x08,0x00,0x51,0x00,0x00,0x00,0x00};
byte Data_Pay_Coin[8] = {0xED,0x08,0x01,0x50,0x00,0x01,0x00,0x00};

byte inQuireCoin[8];

byte CheckSumCoin = 0x00;
byte Sn = 0x01;

byte bufferRxCoin[100];
int indexRxCoin = 0;
int indexReadCoin = 0;

char serialCoinBuffer[100];

void InitCoinPay(void)
{
  Coin.begin(9600); 
  //Serial.begin(9600);
}

void SendDataCoin(byte *data,byte lengths)
{
  for(int i =0;i<lengths;i++){
    Coin.write((char)data[i]);
	Serial.write((char)data[i]);
  }
}
void PayCoin(int num)
{
// 	byte test[2];
// 	test[0] = test[1] = 0xFF;
//   CheckStatusCoin(test);
//   //PacketToRasberryPayCoin(test[0],test[1]);
//   if ((test[0]==STATUS_NOCOIN_NOTFINISH) || (test[0]==STATUS_EMPTYCOIN)||(test[0]==0x29))
//   {
// 	  PacketToRasberryPayCoin(EMPTY,test[1]);
// 	  SendDataCoin(ResetC,8);
// 	  return;
//   }
  
  byte error[2];
  
  error[0] = error[1] =0xFF;
//   CheckStatusCoin(error);
//   if(error[0] != 0x00)
//   {
// 	  PacketToRasberryPayCoin(error[0],8);
// 	  return;
//   }
  
  //Serial.write(0xFE);
 
  
  
  CheckSumCoin=0x00;//clearvalue CheckSum
  Data_Pay_Coin[2]= Sn++;
  Data_Pay_Coin[5]= num;
  
  for(int i =0;i<7;i++){// loop 7
   //Pay_Coin[i] = Data_Pay_Coin[i];   
   CheckSumCoin^=Data_Pay_Coin[i]; 
    
  }
  //Serial.write(CheckSumCoin); 
  Data_Pay_Coin[7] = CheckSumCoin;
  SendDataCoin(Data_Pay_Coin,8);
  

  
  if(Sn==255) Sn=0;  
    
  byte returnCoin[10];
  byte numPacket = WaitCommandCoin(returnCoin,6,5000);
//   byte numPacket = 0;
//   numPacket = WaitCommandCoin(returnCoin,6,5000);
  Serial.write(0xAA);
  Serial.write(numPacket);
  error[0] = error[1] =0xFF;
  if (numPacket > 5)//Packet Rx `0k `6 byte
  {	    
	   error[0] = returnCoin[3];
	   error[1] = returnCoin[5];
	   Serial.write(error[0]);
	   switch(error[0]){
		   case STATUS_NO_ERROR :
				PacketToRasberryPayCoin(OK,error[1]);return;
				break;
		   case STATUS_LOWCOIN :
// 					Serial.write(0xFE);
// 					Serial.write(0x10);
				break;
		   case STATUS_EMPTYCOIN : 
					//PacketToRasberryPayCoin(EMPTY,error[1]);
// 					byte test[2];
// 					test[0] = test[1] = 0xFF;
// 					CheckStatusCoin(test);
// 					delay(1000);
// 					PacketToRasberryPayCoin(test[0],test[1]);
					//SendDataCoin(ResetC,8); 
					//return;					
				break;
		   case STATUS_NOCOIN_NOTFINISH :
// 					 Serial.write(0xFE);
// 					 Serial.write(0x11);
				break;
		   case STATUS_ERROR :
// 					Serial.write(0xFE);
// 					Serial.write(0x13);
					//SendDataCoin(ResetC,8); return;
				//break;
			case 0x40 :
				//SendDataCoin(ResetC,8); return;
			break;
	   }	  
  }
  PacketToRasberryPayCoin(TIMEOUT,5); // time out command
	
}
void SendInquire()
{
	CheckSumCoin=0x00;//clearvalue CheckSum
	for(int i =0;i<8;i++){
		inQuireCoin[i] = Inquire[i];
		if(i==2)
			inQuireCoin[i]=Sn+1;
		if(i==7)
			inQuireCoin[i] = CheckSumCoin;
		CheckSumCoin^=inQuireCoin[i];
	}
	 SendDataCoin(inQuireCoin,8);
	 if(Sn++>255)
	 Sn=0;
	 
}
void ResetPayCoin()
{
	SendDataCoin(ResetC,8);
	PacketToRasberryPayCoin(OK,0xFF);
}
int8_t WaitCommandCoin(byte *expected_answer,byte l ,unsigned int timeout)
{

	uint8_t lengthb=0,  answer=0;
	char response[100];
	unsigned long previous;
	
	while( Coin.available() > 0) Coin.read();
	
	lengthb = 0;
	previous = millis();

	// this loop waits for the answer
	do{
		if(Coin.available() != 0){
			expected_answer[lengthb] = Coin.read();
			Serial.write(expected_answer[lengthb]); //debug command
			lengthb++;
		}
	}while((l!=lengthb) && ((millis() - previous) < timeout));

	return lengthb;
}
void CheckStatusCoin(byte *error)
{	
	SendInquire();
	byte returnCoin1[10];
	byte numPacket1 = WaitCommandCoin(returnCoin1,8,5000); 
	error[0] = 0xFF;error[1] =0xFF;
	if (numPacket1 > 5)
	{	
		error[0] = returnCoin1[3];
		error[1] = returnCoin1[5];
	}else{		
		error[0] = 0x14;//TIMEOUT;
	}
}
void CheckStatusCoinFromrasberriPi(/*byte *error*/)
{
	SendInquire();
	byte returnCoin[10];
	byte numPacket = WaitCommandCoin(returnCoin,8,5000);
	error[0] = 0xFF;error[1] =0xFF;
	if (numPacket > 5)
	{
		error[0] = returnCoin[3];
		error[1] = returnCoin[5];
		}else{
		error[0] = TIMEOUT;
		PacketToRasberryPayCoin(TIMEOUT,5);
	}
	PacketToRasberryPayCoin(error[0],error[1]);
}
void PacketToRasberryPayCoin(byte _status,byte _value)
{
	CheckSumToRasberry03 = 0x00;
        DataToRasberry03[4] = _status;
        DataToRasberry03[5] = _value;
        //DataToRasberry03[6] = CheckSumToRasberry01;
        
	for (int i=2;i<6;i++)
	{
		CheckSumToRasberry03  += DataToRasberry03[i];
	}
	
    DataToRasberry03[6] = ~CheckSumToRasberry03;
	SendDataToRassberry01(DataToRasberry03,7);//================send error to rasberry pi=====================//
}
void testPay(byte num)
{
	CheckSumCoin=0x00;//clearvalue CheckSum
	Data_Pay_Coin[2]= Sn++;
	Data_Pay_Coin[5]= num;	
	for(int i =0;i<7;i++){// loop 7
		CheckSumCoin^=Data_Pay_Coin[i];
		
	}
	//Serial.write(CheckSumCoin);
	Data_Pay_Coin[7] = CheckSumCoin;
	SendDataCoin(Data_Pay_Coin,8);
	if(Sn==255) Sn=0;  
	
// 	 byte returnCoin[10];
// 	 byte numPac = WaitCommandCoin(returnCoin,6,5000);
// 	 Serial.write(numPac);
}