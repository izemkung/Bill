#include <SoftwareSerial.h>
//===============NUMBER OF MACHINE===========//
#define RECIVEMACHINE  01
#define PAYBILLMACHINE 02
#define PAYCOINMACHINE 03
//===========Control Status==========//
#define AcceptBill       0x02
#define RejectBill       0x0F
#define HoldBill         0x18
#define RequestBillStatus 0x0C
//==========Command Respond==============//
#define PowerOn          0x80
#define Request02        0x8F
#define BillOk           0x81

//==========Recive Value Money===========//
#define TwentyBath       0x40
#define FiftyBath        0x41
#define OnehundredBath   0x42
#define FivehundredBath  0x43
#define OnethousandBath  0x44

//============Response Bill=============//
#define MotorFailure     0x20
#define CheckSumError    0x21
#define BillJam          0x22
#define BillRemove       0x23
#define StackerOpen      0x24
#define SensorProblem    0x25
#define CommunicationFailure    0x26
#define BillFish         0x27
#define StackerProblem   0x28
#define BillReject       0x29
#define InvalideCommand  0x2A
#define Revserved        0x2E
#define WhenErrorStatusisExclusion    0x2F
#define BillAcceptorEnableStatus      0x3E
#define BillAcceptorInhibitStatus     0x5E

//===========Control ON OFF Bill======//
#define ControllerEnableBillAcceptor  0x3E
#define ControllerDisableBillAcc	  0x5E

//===============ERROR OF MACHINE===========//
#define NOERROR		0xFE
#define TIMEOUT		0xFD
#define Bill  Serial1

byte DataToRasberry01[7] = {0xFF,0xFF,0x04,0x01,0x00,0x00,0x00}; //===(start1,start2,length,machine,error.status,value,checksum)===//
byte CheckSumToRasberry01 ;
byte ErrorMachineRecive;
byte ValueBill ; 

char powerUp[] = {PowerOn,Request02};
char choice = '/0';

char bufferRx[100];
int indexRx = 0;
int indexRead = 0;

char serialbuffer[1000];
boolean flagBillAcceptor = false;
 


void InitBillRecive(void)
{
  Bill.begin(9600,SERIAL_8E1);  
  delay(2000);
   SendDataToMachine(AcceptBill);
   delay(1000);
   SendDataToMachine(ControllerDisableBillAcc);
   delay(1000);
}

void ReciveBill(void)
{ 
	
  if(flagBillAcceptor == false)return;
  
  EnableReciveBill();//================================== enable machin recive bill===========================//
  byte billType = 0xFF;    
  if(WaitCommand(&billType,20000)) //==================== wait 20sec for frecive bill=====================//
   {
    //imp
     if(billType == BillOk)
    {
           CheckValueBill(billType);
	   CheckStatusReciveBill();	  
    }   
   }else{
   //timeout
	PacketToRasberryReciveBill(TIMEOUT,7);
	DisnableReciveBill();
   } 
   
   //AcceptBill
   if(billType != 0xFF)
   {
	CheckValueBill(billType);
    SendDataToMachine(AcceptBill);   
   }
   flagBillAcceptor = false;
   
}

void SendDataToMachine(byte data)
{
  Bill.write((char)data);
}
void SendDataToRassberry(byte *data,byte lengthR)
{
	for (int i=0;i<lengthR;i++)
	{
		Serial.write((char)data[i]);
	}
}
void CalcRxData()
{  
  while(Bill.available())
  {    
    bufferRx[indexRx++] = Bill.read();
  } 
  
  if(indexRx == indexRead)return;
  int numData = indexRx - indexRead;
  byte data = bufferRx[indexRead++];
  
  switch(data){
          case Request02 : 
              SendDataToMachine(AcceptBill) ;break;
          //case CommunicationFailure : 
              //SendData(AcceptBill); break;
              
          case BillOk : // imperment
              flagBillAcceptor = true ;break;
			  
  }
}

int8_t WaitCommand(byte *expected_answer, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;
    
    while( Bill.available() > 0) Bill.read();  
     
    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Bill.available() != 0){    
            expected_answer[x] = Bill.read();
            x++;           
            answer = 1;
        }
    }while((answer == 0) && ((millis() - previous) < timeout));   

    return answer;
}

boolean ByteArrayCompare(byte a[],byte b[],int array_size)
{
   for (int i = 0; i < array_size; ++i)
     if (a[i] != b[i])
       return(false);
   return(true);
}

void CheckStatusReciveBill()
{
  SendDataToMachine(RequestBillStatus);
  byte Status = 0xFF;
  if(WaitCommand(&Status,1000))
   {
	   switch(Status){
			  case MotorFailure :				ErrorMachineRecive = MotorFailure;  break;
			  case CheckSumError :				ErrorMachineRecive = CheckSumError; break;             
			  case BillJam :					ErrorMachineRecive = BillJam;   break;
			  case BillRemove :					ErrorMachineRecive = BillRemove;  break;
			  case StackerOpen :				ErrorMachineRecive = StackerOpen;  break;          
			  case SensorProblem :				ErrorMachineRecive = SensorProblem;  break;
			  case CommunicationFailure :		ErrorMachineRecive = CommunicationFailure; break;
			  case BillFish :					ErrorMachineRecive = BillFish;  break;          
			  case StackerProblem :				ErrorMachineRecive = StackerProblem;	  break;
			  case BillReject :					ErrorMachineRecive = BillReject;  break;
			  case InvalideCommand :			ErrorMachineRecive = InvalideCommand;  break;             
			  case Revserved :					ErrorMachineRecive = Revserved; break;
			  case WhenErrorStatusisExclusion : ErrorMachineRecive = WhenErrorStatusisExclusion; break;
			  case BillAcceptorEnableStatus :   ErrorMachineRecive = BillAcceptorEnableStatus; break;          
			  case BillAcceptorInhibitStatus :  flagBillAcceptor = true ; break;
			  default:ErrorMachineRecive = NOERROR;break;
		  }
   }else{
     //==========time out of checkerror=================//
		ErrorMachineRecive = 0xFD;
   } 
    PacketToRasberryReciveBill(ErrorMachineRecive,7);
   
  
}
void CheckValueBill(byte data)
{
    switch(data)
    {
        case TwentyBath :		ValueBill = TwentyBath ; break;
        case FiftyBath :		ValueBill = FiftyBath ; break;
        case OnehundredBath :	ValueBill = OnehundredBath ; break;
        case FivehundredBath :	ValueBill = FivehundredBath ; break;
        case OnethousandBath :	ValueBill = OnethousandBath ; break;
    }
	PacketToRasberryReciveBill(ValueBill,7);
}
void EnableReciveBill()
{
	SendDataToMachine(ControllerEnableBillAcceptor);
}
void DisnableReciveBill()
{
	SendDataToMachine(ControllerDisableBillAcc);
}

void PacketToRasberryReciveBill(byte status,byte lengthR)
{
	CheckSumToRasberry01 = 0x00;
	for (int i=0;i<lengthR;i++)
	{
		if (i==4)
		{
			DataToRasberry01[i] = status;
		}
		if (i==6)
		{
			DataToRasberry01[i] = CheckSumToRasberry01;
		}
		CheckSumToRasberry01 ^= DataToRasberry01[i];
	}
	SendDataToRassberry(DataToRasberry01,7);//================send error to rasberry pi=====================//
}
