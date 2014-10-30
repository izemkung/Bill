#include <SoftwareSerial.h>


//==========Status Coin========================//
#define STATUS_READY               0x00
#define STATUS_LOWCOIN             0x20
#define STATUS_NOCOIN_NOTFINISH    0x2A
#define STATUS_EMPTYCOIN           0x2B
#define STATUS_PAYCOIN_ERROR       0x09


#define Coin Serial3
//===================Command Control============//
byte ResetCoin[8] = {0xED,0x08,0x00,0x50,0x07,0x00,0x00,0xB2};
byte Inquire[8] = {0xED,0x08,0x00,0x51,0x07,0x00,0x00,0x00};
byte Data_Pay_Coin[8] = {0xED,0x08,0x00,0x50,0x07,0x01,0x00,0x00};
byte Pay_Coin[8];
byte CheckSumCoin = 0x00;
byte Sn = 0x00;

char bufferRxCoin[100];
int indexRxCoin = 0;
int indexReadCoin = 0;

char serialCoinBuffer[100];

void InitCoinPay(void)
{
  Coin.begin(9600); 
  Serial.begin(9600);
}
void SendDataCoin(byte data[])
{
  for(int i =0;i<8;i++){
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
    CheckSumCoin+=Pay_Coin[i];
    Sn++;
  }
  SendDataCoin(Pay_Coin);
  if(Sn>255)
    Sn=0;
  CheckSumCoin=0x00;
  
}
int8_t WaitCommandCoin(byte *expected_answer, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;
    
    while( Coin.available() > 0) Coin.read();  
     
    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Coin.available() != 0){    
            expected_answer[x] = Coin.read();
            x++;           
            answer = 1;
        }
    }while((answer == 0) && ((millis() - previous) < timeout));   

    return answer;
}
void CalcRxDataCoin()
{  
  while(Coin.available())
  {    
    bufferRxCoin[indexRxCoin++] = Coin.read();
  }    
  if(indexRxCoin == indexReadCoin)return;
  int numData = indexRxCoin - indexReadCoin;
 // byte data = bufferRxCoin[indexReadCoin++];
 for(int i=0; i< numData;i++) 
 {
   serialCoinBuffer[i] = bufferRxCoin[i];
 }
 //SendDataCoin(serialbufferCoin);
  
  
}
