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
#define NoReceivedWithin2milisec 0x26

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
#define BillFish         0x27
#define StackerProblem   0x28
#define BillReject       0x29
#define InvalideCommand  0x2A
#define Revserved        0x2E
#define WhenErrorStatusisExclusion    0x2F
#define BillAcceptorEnableStatus      0x3E
#define BillAcceptorInhibitStatus     0x5E

char buff = Request02;
char serialbuffer[1000];
SoftwareSerial mySerial(10, 11);// RX, TX
void Init(void)
{
  Serial.begin(9600,SERIAL_8E1); 
  mySerial.begin(9600);
  Serial.println("hello");  
}
void ReciveBill(void)
{ 
 while(1)
  {
    if(mySerial.available())
    {  
       Serial.print("recive");  
     if (mySerial.find(&buff))     
        mySerial.write(AcceptBill); 
        Serial.println("AcceptBill");       
    }  
  } 
}

