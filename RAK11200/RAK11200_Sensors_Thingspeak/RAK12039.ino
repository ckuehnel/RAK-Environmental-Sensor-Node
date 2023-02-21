// RAK12039

boolean initRAK12039(void)
{
  pinMode(WB_IO6, OUTPUT);  // WB_IO6 is connected to the SET pin of RAK12039_PMSA003I
  digitalWrite(WB_IO6, HIGH);
  delay(300); 
  if(PMSA003I.begin()) 
    RAK12039_ready = true;
  else 
  {
    Serial.println("RAK12039 not detected");
    RAK12039_ready = false;
  }
  return RAK12039_ready;
}

void readRAK12039(void)
{
  PMSA_Data_t data;
  
  if (PMSA003I.readDate(&data)) 
  {
    RAK12039_PM10 = data.pm10_env;
    RAK12039_PM25 = data.pm25_env;
    RAK12039_PM100 = data.pm100_env;
    if (DEBUG) Serial.printf("RAK12039: PM1.0 = %3d µg/𝑚3, PM2.5 = %3d µg/𝑚3, PM10 = %3d µg/𝑚3\n", RAK12039_PM10, RAK12039_PM25, RAK12039_PM100);
      
  }
 else Serial.println("PMSA003I read failed!");
}
