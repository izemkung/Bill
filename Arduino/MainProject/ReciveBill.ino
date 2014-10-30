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
#define ControllerDisableBillAcc

#define Bill  Serial1

char powerUp[] = {PowerOn,Request02};
char choice = '/0';

char bufferRx[100];
int indexRx = 0;
int indexRead = 0;

char serialbuffer[1000];
SoftwareSerial mySerial(10, 11);// RX, TX

boolean flagBillAcceptor = false;
 


void InitBillRecive(void)
{
  Bill.begin(9600,SERIAL_8E1); 
  mySerial.begin(9600);
  Bill.println("hello"); 
  
  delay(2000);
  SendData(AcceptBill);
  delay(1000);
  SendData(BillAcceptorInhibitStatus); 
  //delay(9000);
  //SendData(BillAcceptorEnableStatus); 
}

void ReciveBill(void)
{ 
  if(flagBillAcceptor == false)return;
  byte billType = 0xFF;
    
  if(WaitCommand(&billType,200))
   {
    //imp
    // if(billType == BillOk)
    // {
    //    CheckValueBill(billType);
    // }
   
   }else{
   //timeout
   } 
   
   //AcceptBill
   if(billType != 0xFF)
     SendData(AcceptBill);
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
              //SendData(AcceptBill); break;
          case CommunicationFailure : 
              //SendData(AcceptBill); break;
              
          case BillOk : // imperment
              flagBillAcceptor = true ; break;
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
<<<<<<< HEAD:Arduino/MainProject/ReciveBill.ino
  byte Status = 0xFF;
  if(WaitCommand(&Status,200))
   {
      switch(Status){
=======
 // ส่ง คำร้องขอไป เเล้วเครื่องจะส่งข้อมูลกลับมา
 // data คือข้อมูลที่ถูกส่งกลับมา เเล้วจะเอามาตรวจสอบได้ยังไง
   switch(data){
>>>>>>> ac0d0eabdfaac776631a7e8352a0f9a232b4f8d0:Arduino/MainProject/ReciveMoney.ino
          case MotorFailure : 
              //SendData(MotorFailure); break;
          case CheckSumError : 
              //SendData(CheckSumError); break;              
          case BillJam : 
              //SendData(BillJam) ; break;
          case BillRemove : 
             // SendData(BillRemove); break;
          case StackerOpen : 
              //SendData(StackerOpen); break;              
          case SensorProblem : 
              //SendData(SensorProblem) ; break;
          case CommunicationFailure : 
              //SendData(CommunicationFailure); break;
          case BillFish : 
              //SendData(BillFish); break;              
          case StackerProblem : 
             //SendData(StackerProblem) ; break;
          case BillReject : 
             // SendData(BillReject); break;
          case InvalideCommand : 
              //SendData(InvalideCommand); break;              
          case Revserved : 
               //SendData(Revserved) ; break;
          case WhenErrorStatusisExclusion : 
             // SendData(WhenErrorStatusisExclusion); break;
          case BillAcceptorEnableStatus : 
             // SendData(BillAcceptorEnableStatus); break;              
          case BillAcceptorInhibitStatus : // imperment
              flagBillAcceptor = true ; break;
      }
   }else{
   //timeout
   } 
   
  
}
void CheckValueBill(byte data)
{
   if(WaitCommand(&data,200)){
      switch(data){
          case TwentyBath : 
          //SendData(TwentyBath); break;
          case FiftyBath : 
          //SendData(FiftyBath); break;
          case OnehundredBath : 
          //SendData(OnehundredBath); break;
          case FivehundredBath : 
          //SendData(FivehundredBath); break;
          case OnethousandBath : 
          SendData(OnethousandBath); break;
      }
   }
}
void SendToRasberry()
{
  //packet to rasbrary pi
}
