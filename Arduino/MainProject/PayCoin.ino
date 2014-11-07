//#include <SoftwareSerial.h>


//==========Status Coin========================//
#define STATUS_NO_ERROR            0x00
#define STATUS_LOWCOIN             0x20
#define STATUS_NOCOIN_NOTFINISH    0x21
#define STATUS_EMPTYCOIN           0x28
#define STATUS_PAYCOIN_ERROR       0x29


#define Coin Serial2
//===================Command Control============//
byte ResetC[8] = {0xED,0x08,0x00,0x50,0x00,0x00,0x00,0xB2};
byte Inquire[8] = {0xED,0x08,0x00,0x51,0x00,0x00,0x00,0x00};
byte Data_Pay_Coin[8] = {0xED,0x08,0x01,0x50,0x00,0x01,0x00,0x00};
byte Pay_Coin[8];
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
	CheckSumCoin=0x00;//clearvalue CheckSum
  for(int i =0;i<8;i++){
   Pay_Coin[i] = Data_Pay_Coin[i];   
   if(i==2)
   {
     Pay_Coin[i]=Sn;
   }
   if(i==5)
    {
	Pay_Coin[i]= num;
	}
   if(i==7)
     {
	 Pay_Coin[i] = CheckSumCoin;
	 }
	CheckSumCoin^=Pay_Coin[i];   
  }
  SendDataCoin(Pay_Coin,8);
  if(Sn++>255)
    Sn=1;  
  byte returnCoin[10];
  byte numPacket = WaitCommandCoin(returnCoin,8,5000);
  byte error1 = 0xFF;byte error2 =0xFF;
  if (numPacket > 5)
  {
	   error1 = returnCoin[3];
	   for(int i=0; i<numPacket;i++){
		Serial.write((char)returnCoin[i]);
	   }
	}else{
		//ตอบกลับไม่มีการตอบสนอง  
		CheckStatusCoin(); 
	}
	if (error1 == STATUS_EMPTYCOIN)
	{
		//CheckStatusCoin();
	}
	if (error1==STATUS_NOCOIN_NOTFINISH || error1 == STATUS_PAYCOIN_ERROR )
	{
		CheckStatusCoin();
	}
  //Send STATUS_NO_ERROR
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
			lengthb++;
		}
	}while((l!=lengthb) && ((millis() - previous) < timeout));

	return lengthb;
}
void CheckStatusCoin()
{	
	SendInquire();
	byte returnCoin[10];
	byte numPacket = WaitCommandCoin(returnCoin,8,5000); 
	byte error1 = 0xFF;byte error2 =0xFF;
	if (numPacket > 5)
	{	
		error1 = returnCoin[3];
		error2 = returnCoin[5];
		for (int i =0;i<numPacket;i++)
		{
			Serial.write((char)returnCoin[i]);	
		}
		
	
		//Serial.println("CheckStatusCoin  ");
// 		switch(error1)
// 		{
// 			case STATUS_LOWCOIN :
// 			Serial.println("STATUS_LOWCOIN  ");break;
// 			case STATUS_EMPTYCOIN :
// 			Serial.println("STATUS_EMPTYCOIN  ");
// 			Serial.write((char)error2);break;
// 			case STATUS_PAYCOIN_ERROR :
// 			Serial.println("STATUS_PAYCOIN_ERROR ");break;	
// 			default:Serial.println("NO_ERROR ");break;			
// 		}
	}else{
		//ตอบกลับไม่มีการตอบสนอง
		//Serial.println("CheckStatusCoin timeout");
	}
	//send to rasberrry pi
}
