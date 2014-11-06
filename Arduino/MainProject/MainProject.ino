
void setup()
{
  //InitBillRecive();
 // InitBillPay();
  InitCoinPay();
  Serial.begin(9600);
 // Serial.println("Hello");
}

void loop() 
{
  
  //CalcRxData();
 //ReciveBill();

//ResetPayBill();
//delay(5000);
// Paybill(5);
//  CheckStatusPayBill();

 PayCoin(1);
 delay(1000);
 PayCoin(1);
 while(1);
  
}
