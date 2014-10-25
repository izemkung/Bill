
void setup()
{
  Init();
}
void loop() 
{
  while(Serial.available())
  {    
    bufferRx[indexRx++] = Serial.read();
  }  
  CalcRxData();
}
