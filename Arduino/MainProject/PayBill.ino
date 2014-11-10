//#include <SoftwareSerial.h>

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

#define payBill Serial3


//====================Command===========================//
byte CheckPayBill[6] = {0x01,0x10,0x00,0x11,0x00,0x22};
byte PayBillReset[6] =       {0x01,0x10,0x00,0x12,0x00,0x23};
byte Datapay[6] =            {0x01,0x10,0x00,0x10,0x01,0x22};
byte pay[6];
byte error[6];


int array_size = 6;
byte Checksum ;
byte count1 = 0x00;
byte count2 = 0x00;

char serialBillbuffer[100];
char bufferRxBill[100];
int indexRxBill = 0;
int indexReadBill = 0;



void InitBillPay(void)
{
  payBill.begin(9600,SERIAL_8E1); 
}
void SendDataPayBill(byte *data,byte lengthb)
{
    for(int i=0; i<lengthb;i++){
     payBill.write((char)data[i]);
	 Serial.write((char)data[i]);	 
    }
}

void Paybill(int num) 
{
	Checksum = 0x00;
   for(int i =0;i<6;i++){
     pay[i] = Datapay[i];	    
     if(i==4)
       pay[i]=num;     
	 if(i==5)     
       pay[i] = Checksum; 
     Checksum ^= pay[i];
   }
    SendDataPayBill(pay,6);   
    byte returnBill[10];
    byte numPacketBill = WaitCommandBill(returnBill,6,5000);
    byte error = 0xFF;
    if (numPacketBill > 5)
    {
		for(int i=0; i<6;i++){
			//payBill.write((char)data[i]);
			 Serial.write((char)returnBill[i]);			
		}	
	}else{
	   //ตอบกลับไม่มีการตอบสนอง
	    Serial.println("Paybill Time Out");
		//CheckStatusPayBill();
    }
	
   
}
void CheckStatusPayBill()
{
  SendDataPayBill(CheckPayBill,6);
  byte returnBill[10];
  byte numPacketBill = WaitCommandBill(returnBill,6,5000);
  byte error1 = 0xFF;
  if (numPacketBill > 5)
  {
	  for(int i=0; i<6;i++){
		  //payBill.write((char)data[i]);
		  Serial.write((char)returnBill[i]);		  
	  }
	  error1 = returnBill[3];
	  CheckStatus(error1);
  }else{
	  //ตอบกลับไม่มีการตอบสนอง
	   Serial.println("CheckStatusPayBill Time Out ");
  }
}
int8_t WaitCommandBill(byte *expected_answer,byte l ,unsigned int timeout)
{

	uint8_t lengthb=0,  answer=0;
	char response[100];
	unsigned long previous;
	
	while( payBill.available() > 0) payBill.read();
	
	lengthb = 0;
	previous = millis();

	// this loop waits for the answer
	do{
		 if(payBill.available() != 0){  
			expected_answer[lengthb] = payBill.read();
			lengthb++;
		 }
	}while((l!=lengthb) && ((millis() - previous) < timeout));

	return lengthb;
}
void ResetPayBill()
{
	SendDataPayBill(PayBillReset,6);
}
void CheckStatus(byte data)
{
  switch(data)
  {
    case PayOutSuccessful :
    Serial.println("PayOutSuccessful");break;
    case PayOutFail :
      Serial.println("PayOutFail");break;
    case StatusFine : //ready paybill
     Serial.println("StatusFine");break;
    case EmptyNote :
      Serial.println("EmptyNote");break;
    case StockLess :
     Serial.println("StockLess");break;
    case NoteJam : 
     Serial.println("NoteJam");break;
    case OverLength :
    Serial.println("OverLength");break;
    case NoteNotExit :
    Serial.println("NoteNotExit");break;
    case SensorError :
    Serial.println("SensorError");break;
    case DoubleNoteError :
     Serial.println("DoubleNoteError");break;
    case MotorError :
     Serial.println("MotorError");break;
    case DispensingBusy :
     Serial.println("DispensingBusy");break;
    case SensorAdjusting :
     Serial.println("SensorAdjusting");break;
    case CheckSumError : 
     Serial.println("CheckSumError");break;
    case LowPowerError :
     Serial.println("LowPowerError");break;
  }
}


