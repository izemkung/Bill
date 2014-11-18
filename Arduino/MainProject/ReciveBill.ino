
//===============NUMBER OF MACHINE===========//
#define RECIVEMACHINE  01
#define PAYBILLMACHINE 02
#define PAYCOINMACHINE 03
//===========Control Status==========//
#define AcceptBill       0x02
#define RejectBill       0x0F
#define HoldBill         0x18
#define RequestBillStatus 0x0C
#define RESETRECIVEBILL 0x30
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
}

void ReciveBill(void)
{ 
  //ResetMachineReciveBill();

  //if(flagBillAcceptor == false){PacketToRasberryReciveBill(TIMEOUT,7);Serial.println("flagfalse");return;}
	
	//Serial.println("flagtrue");
  EnableReciveBill();
  //delay(500);
  
  byte flagOK;
  
  flagOK =  CheckStatusReciveBill();
  if (flagOK == BillAcceptorEnableStatus)
  {
	//EnableReciveBill();
	//ResetMachineReciveBill(); 
  }else{
	  PacketToRasberryReciveBill(flagOK,7);
	  DisnableReciveBill();	
      return;
  } 
   
  byte billType = 0xFF;
 
  if(WaitCommand(&billType,30000)) //wait 30sec 
   {	
	if(billType == BillOk)//if 0x81 judre Bill ok
	{
	  if(WaitCommand(&billType,2000))//wait bill type
          {
			 char _billType = billType;
			 SendDataToMachine(AcceptBill);
			 
			 if(WaitCommand(&billType,2000))
			 {
				if(billType == 0x10)//Stacking
					PacketToRasberryReciveBill(CheckValueBill(_billType),7);
				else
					PacketToRasberryReciveBill(0x11,7);//reject
			 }
             //if raspberry accept  
          }else{
            PacketToRasberryReciveBill(TIMEOUT,7);//
          }			
	}else{
		 PacketToRasberryReciveBill(billType,7);//no bill type
	}
	
   }else{
      PacketToRasberryReciveBill(TIMEOUT,7);//no judge bill
   }  
   
   delay(500); 
   DisnableReciveBill();	
   	
   //flagBillAcceptor = false;
   
}

void SendDataToMachine(byte data)
{
  Bill.write((char)data);
}
void SendDataToRassberry01(byte *data,byte lengthR)
{
	for (int i=0;i<lengthR;i++)
	{
		Serial.write((char)data[i]);
	}
}
void CalcRxDataBillRecive()
{  
  while(Bill.available())
  {    
    bufferRx[indexRx++] = Bill.read();
  } 
  
  if(indexRx == indexRead)return;
  int numData = indexRx - indexRead;
  byte data = bufferRx[indexRead++];
  //Serial.write(0xff);
  //Serial.write(data);
  
  
  switch(data){
          case Request02 : 				
              SendDataToMachine(AcceptBill) ;
			  delay(2000);
              SendDataToMachine(ControllerDisableBillAcc);break;
//           case CommunicationFailure : 
//           SendDataToMachine(AcceptBill); break;
              
          case BillOk : // imperment
              
              break;
			  
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
			 //Serial.write(0xfe);
			 //Serial.write(expected_answer[x]);
			if (expected_answer[x] != 0x00)
			{
				x++;
				answer = 1;
			}
            
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

byte CheckStatusReciveBill()
{
  SendDataToMachine(RequestBillStatus);
  byte Status = 0xFF;
  if(WaitCommand(&Status,5000))
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
			  case BillAcceptorInhibitStatus :   ; break;
			  default:ErrorMachineRecive = NOERROR;break;
		  }
   }else{
     //==========time out of checkerror=================//
		ErrorMachineRecive = 0xFD;
   } 
   // PacketToRasberryReciveBill(ErrorMachineRecive,7);
   return ErrorMachineRecive;
  
}
byte CheckValueBill(byte data)
{
    switch(data)
    {
        case TwentyBath :		ValueBill = TwentyBath ; break;
        case FiftyBath :		ValueBill = FiftyBath ; break;
        case OnehundredBath :	ValueBill = OnehundredBath ; break;
        case FivehundredBath :	ValueBill = FivehundredBath ; break;
        case OnethousandBath :	ValueBill = OnethousandBath ; break;
    }
	//PacketToRasberryReciveBill(ValueBill,7);
	return ValueBill;
}
void EnableReciveBill()
{
	SendDataToMachine(ControllerEnableBillAcceptor);
}
void DisnableReciveBill()
{
	SendDataToMachine(ControllerDisableBillAcc);
}
void ResetMachineReciveBill()
{
        flagBillAcceptor = false;
	SendDataToMachine(RESETRECIVEBILL);	
}
void PacketToRasberryReciveBill(byte _status,byte lengthR)
{
	CheckSumToRasberry01 = 0x00;
        DataToRasberry01[4] = _status;
        //DataToRasberry01[6] = CheckSumToRasberry01;
	for (int i=2;i<6;i++)
	{
		CheckSumToRasberry01  += DataToRasberry01[i];
	}
	
        DataToRasberry01[6] = ~CheckSumToRasberry01;
	SendDataToRassberry01(DataToRasberry01,7);
}
byte checkSumReciveBill(byte _start,byte _stop)
{
	byte check_Sum = 0;
	for(;_start != _stop;  _start++)
	{
		check_Sum  += DataToRasberry01[_start];
	}
	return ~check_Sum;
}
