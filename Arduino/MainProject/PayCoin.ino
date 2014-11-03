#include <SoftwareSerial.h>


//==========Status Coin========================//
#define STATUS_NO_ERROR               0x00
#define STATUS_LOWCOIN             0x20
#define STATUS_NOCOIN_NOTFINISH    0x2A
#define STATUS_EMPTYCOIN           0x2B
#define STATUS_PAYCOIN_ERROR       0x09


#define Coin Serial2
//===================Command Control============//
byte ResetCoin[8] = {0xED,0x08,0x00,0x50,0x07,0x00,0x00,0xB2};
byte Inquire[8] = {0xED,0x08,0x00,0x51,0x07,0x00,0x00,0x00};
byte Data_Pay_Coin[8] = {0xED,0x08,0x00,0x50,0x07,0x01,0x00,0x00};
byte Pay_Coin[8];
byte inQuireCoin[8];

byte CheckSumCoin = 0x00;
byte Sn = 0x00;

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
  }
}
void PayCoin(int num)
{
  for(int i =0;i<8;i++){
   Pay_Coin[i] = Data_Pay_Coin[i];   
   if(i==2)
     Pay_Coin[i]=Sn;
   if(i==5)
    Pay_Coin[i]= num;
   if(i==7)
     Pay_Coin[i] = CheckSumCoin;
    CheckSumCoin^=Pay_Coin[i];   
  }
  SendDataCoin(Pay_Coin,8);
  if(Sn++>255)
    Sn=0;
  CheckSumCoin=0x00;//clearvalue CheckSum
  
  byte returnCoin[10];
  byte numPacket = WaitCommandCoin(returnCoin,200);
  byte error1 = 0xFF;byte error2 =0xFF;
  if (numPacket > 5)
  {
	  error1 = returnCoin[3];
	  
	  }else{
		//ตอบกลับไม่มีการตอบสนอง  	  
	}
	if (error1==STATUS_LOWCOIN || error1 == STATUS_NOCOIN_NOTFINISH )
	{
		CheckStatusCoin();
	}
  //Send STATUS_NO_ERROR
}
void SendInquire()
{
	for(int i =0;i<8;i++){
		inQuireCoin[i] = Inquire[i];
		if(i==2)
			inQuireCoin[i]=Sn;
		if(i==7)
			inQuireCoin[i] = CheckSumCoin;
		CheckSumCoin^=inQuireCoin[i];
	}
	 SendDataCoin(inQuireCoin,8);
	 if(Sn++>255)
	 Sn=0;
	 CheckSumCoin=0x00;//clearvalue CheckSum
}
int8_t WaitCommandCoin(byte *expected_answer, unsigned int timeout)
{

    uint8_t lengths=0,  answer=0;
    char response[100];
    unsigned long previous;
    
    while( Coin.available() > 0) Coin.read();  
     
    lengths = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        //if(Coin.available() != 0){    
            expected_answer[lengths] = Coin.read();
            lengths++;           
            //answer = 1;
       // }
    }while((Coin.available()) && ((millis() - previous) < timeout));   

    return lengths;
}
void CalcRxDataCoin()
{  
  while(Coin.available())
  {    
    bufferRxCoin[indexRxCoin++] = Coin.read();
  }    
  if(indexRxCoin == indexReadCoin)return;
  int numData = indexRxCoin - indexReadCoin;
 for(int i=0; i< numData;i++) 
 {
   serialCoinBuffer[i] = bufferRxCoin[i];
 }  
}

void CheckStatusCoin()
{
	SendInquire();
	byte returnCoin[10];
	byte numPacket = WaitCommandCoin(returnCoin,200); 
	byte error1 = 0xFF;byte error2 =0xFF;
	if (numPacket > 5)
	{	
		error1 = returnCoin[3];
		error2 = returnCoin[5];
	}else{
		//ตอบกลับไม่มีการตอบสนอง
	}
	 switch(error1){
		 case STATUS_LOWCOIN : 
		 //send error
				break;
		 case STATUS_EMPTYCOIN :
		// send จ่ายเหรียญไม่ครบแต่จ่ายไป7เหรียญ (0x2B,0x07)		
				break;
		 case STATUS_PAYCOIN_ERROR :
		 // send เหรียญไม่หมดแต่จ่ายไปแล้ว7เหรียญ (0x09,0x07)
				break;
				 
	 }
	//Send packet error to Rasberry PI;
}
