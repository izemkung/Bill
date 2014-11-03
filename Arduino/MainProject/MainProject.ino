
void setup()
{
  //InitBillRecive();
  InitBillPay();
  //InitCoinPay();
  Serial.begin(9600);
 // Serial.println("Hello");
}

void loop() 
{
  
  //CalcRxData();
 //ReciveBill();

ResetPayBill();
delay(5000);
 Paybill(5);
  CheckStatusPayBill();
 while(1);
 //PayCoin(2);

  
}
