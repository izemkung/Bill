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

char bufferRxMain[100];
int indexRxMain = 0;
int indexReadMain = 0;

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
	if (Serial.available())
	{
		flagCheckStatus=true;
	}
	LoopCheckStatus();
// 	CalcRxData();
// 	ReciveBill();
  
}
void CalcRxDataMain()
{
	while(Serial.available())
	{
		bufferRxMain[indexRxMain++] = Serial.read();
	}
		
	if(indexRxMain == indexReadMain)return;
	int numDatalength = indexRxMain - indexReadMain;
	//byte data = bufferRx[indexReadMain++];
	for (int i =0;i<numDatalength;i++)
	{
		ReadFromRasberry[i] = bufferRxMain[i];
	}
	ClearBufferMain();
	
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
	if(flagCheckStatus==true)return;
	 unsigned long currentMillis = millis();	 
	 if(currentMillis - previousMillis >= interval) {
		 // save the last time you blinked the LED
		 previousMillis = currentMillis;
		 // ============================make check status====================================
		 
	 }
}
void ClearBufferMain()
{
	for (int i =0;i<100;i++)
	{
		bufferRxMain[i] = '\0';
	}
}