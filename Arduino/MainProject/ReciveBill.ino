#include <SoftwareSerial.h>
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

#define Bill  Serial1

char powerUp[] = {PowerOn,Request02};
char choice = '/0';

char bufferRx[100];
int indexRx = 0;
int indexRead = 0;

char serialbuffer[1000];
//SoftwareSerial mySerial(10, 11);// RX, TX

boolean flagBillAcceptor = false;
 


void InitBillRecive(void)
{
  Bill.begin(9600,SERIAL_8E1);  
  delay(2000);
  //SendData(AcceptBill);
  //delay(1000);
}

void ReciveBill(void)
{ 
  if(flagBillAcceptor == false)return;
  byte billType = 0xFF;    
  if(WaitCommand(&billType,5000))
   {
    //imp
     if(billType == BillOk)
    {
        CheckValueBill(billType);
    }
   
   }else{
   //timeout
   Serial.println("ReciveBill time Out");
   } 
   
   //AcceptBill
   if(billType != 0xFF)
   {
	CheckValueBill(billType);
     SendData(AcceptBill);
   
   }
   flagBillAcceptor = false;
}

void SendData(byte data)
{
  Bill.write((char)data);
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
              SendData(AcceptBill) ;break;
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
  SendData(RequestBillStatus);

  byte Status = 0xFF;
  if(WaitCommand(&Status,200))
   {
   switch(Status){

          case MotorFailure : 
              //SendData(MotorFailure); 
			  Serial.println("MotorFailure  ");
			  break;
          case CheckSumError : 
              //SendData(CheckSumError); break; 
			   Serial.println("CheckSumError  ");
			   break;             
          case BillJam : 
              //SendData(BillJam) ; break;
			   Serial.println("BillJam  ");
			   break;
          case BillRemove : 
             // SendData(BillRemove); break;
			  Serial.println("BillRemove  ");
			  break;
          case StackerOpen : 
              //SendData(StackerOpen); break;    
			   Serial.println("StackerOpen  ");
			   break;          
          case SensorProblem : 
              //SendData(SensorProblem) ; break;
			   Serial.println("SensorProblem  ");
			   break;
          case CommunicationFailure : 
              //SendData(CommunicationFailure); break;
			   Serial.println("CommunicationFailure  ");
			   break;
          case BillFish : 
              //SendData(BillFish); break;    
			   Serial.println("BillFish  ");
			   break;          
          case StackerProblem : 
             //SendData(StackerProblem) ; break;
			  Serial.println("StackerProblem  ");
			  break;
          case BillReject : 
             // SendData(BillReject); break;
			  Serial.println("BillReject  ");
			  break;
          case InvalideCommand : 
              //SendData(InvalideCommand); break; 
			   Serial.println("InvalideCommand  ");
			   break;             
          case Revserved : 
               //SendData(Revserved) ; break;
			    Serial.println("Revserved  ");
			    break;
          case WhenErrorStatusisExclusion : 
             // SendData(WhenErrorStatusisExclusion); break;
			  Serial.println("WhenErrorStatusisExclusion  ");
			  break;
          case BillAcceptorEnableStatus : 
             // SendData(BillAcceptorEnableStatus); break;    
			  Serial.println("BillAcceptorEnableStatus  ");
			  break;          
          case BillAcceptorInhibitStatus : // imperment
              flagBillAcceptor = true ; break;
      }
   }else{
   //timeout
   } 
   
  
}
void CheckValueBill(byte data)
{
    switch(data){
        case TwentyBath : 
        //SendData(TwentyBath); break;
		Serial.println("TwentyBath  "); break;
        case FiftyBath : 
        //SendData(FiftyBath); break;
		Serial.println("FiftyBath  "); break;
        case OnehundredBath : 
        //SendData(OnehundredBath); break;
		Serial.println("OnehundredBath  "); break;
        case FivehundredBath : 
        //SendData(FivehundredBath); break;
		Serial.println("FivehundredBath  ");break;
        case OnethousandBath : 
        // SendData(OnethousandBath); break;
		Serial.println("OnethousandBath  ");break;
    }
}
void OpenReciveBill()
{
	SendData(ControllerEnableBillAcceptor);
}
void DisnableReciveBill()
{
	SendData(ControllerDisableBillAcc);
}
void SendToRasberry()
{
  //packet to rasbrary pi
}
