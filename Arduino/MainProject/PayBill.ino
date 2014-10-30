#include <SoftwareSerial.h>

//=============CommandStatus[4]===============//
#define PayBill           0x10
#define Request           0x11
#define Reset             0x12
#define MultiplePayOut    0x13
#define PayOutSuccessful  0xAA
#define PayOutFail        0xBB
#define StatusFine        0x00
#define EmptyNote         0x01
#define StockLess         0x02
#define NoteJam           0x03
#define OverLength        0x04
#define NoteNotExit       0x05
#define SensorError       0x06
#define DoubleNoteError   0x07
#define MotorError        0x08
#define DispensingBusy    0x09
#define SensorAdjusting   0x0A
#define CheckSumError     0x0B
#define LowPowerError     0x0C

//=====================Number of Bank===============//
#define One    0x01
#define Two    0x02
#define Tree   0x03
#define Four   0x04
#define Five   0x05
#define Six    0x06
#define Seven  0x07
#define Eight  0x08
#define Nine   0x09

#define PayBill Serial2


//====================Command===========================//
byte CheckStatusPayBill[6] = {0x01,0x10,0x00,0x11,0x00,0x22};
byte PayBillReset[6] =       {0x01,0x10,0x00,0x12,0x00,0x23};
byte Datapay[6] =            {0x01,0x10,0x00,0x10,0x01,0x22};
byte pay[6];
byte error[6];


int array_size = 6;
byte Checksum = 0x00;
byte count1 = 0x00;
byte count2 = 0x00;

char serialBillbuffer[100];
char bufferRxBill[100];
int indexRxBill = 0;
int indexReadBill = 0;



void InitBillPay(void)
{
  PayBill.begin(9600,SERIAL_8E1); 
}
void SendDataBill(byte data[])
{
    for(int i=0; i<6;i++){
     PayBill.write((char)data[i]);
    }
}

void Paybill(int num) 
{
   for(int i =0;i<6;i++){
     pay[i] = Datapay[i];     
     if(i==4){
       pay[i]=num;
     }
     if(i==5)
     {
       pay[i] = Checksum;
     }
     Checksum+=pay[i];
   }
   SendDataBill(pay);
}
byte CheckMachineStatus()
{
  SendDataBill(CheckStatusPayBill);
  for(int i =0;i<6;i++){
    error[i] = bufferRxBill[i];
  }
  return error[3];
}

void CalcRxBill()
{
  
  while(PayBill.available())
  {    
    bufferRxBill[indexRxBill++] = PayBill.read();
  }  
  
  if(indexRxBill == indexReadBill)return;
  int numDataBill = indexRxBill - indexReadBill;
  //byte dataBill = bufferRxBill[indexReadBill++];
}
void CheckStatus(byte data)
{
  switch(data)
  {
    case PayOutSuccessful :
    //sendError;break;
    case PayOutFail :
     //sendError;break;
    case StatusFine : //ready paybill
     //sendError;break;
    case EmptyNote :
     //sendError;break;
    case StockLess :
    //senderror
    case NoteJam : 
    //senderror
    case OverLength :
    //senderror
    case NoteNotExit :
    //senderror
    case SensorError :
   //senderror
    case DoubleNoteError :
    //senderror
    case MotorError :
    //senderror
    case DispensingBusy :
    //senderror
    case SensorAdjusting :
    //senderror
    case CheckSumError : 
    //senderror
    case LowPowerError :
    break;
  }
}


