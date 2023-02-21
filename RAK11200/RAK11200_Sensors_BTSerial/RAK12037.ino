// RAK12037

boolean initRAK12037(void)
{
  if (airSensor.begin())
  { 
    RAK12037_ready = true;
    
    if (DEBUG)
    {
      Serial.print("Automatic self-calibration set to：");
      if (airSensor.getAutoSelfCalibration() == true)
        Serial.println("true");
      else
        Serial.println("false");       
    }
  }
  else 
    Serial.println("RAK12047 not detected.");
  
  return RAK12037_ready;
}

void readRAK12037(void)
{
  if(RAK12037_ready)
  {
    if (airSensor.dataAvailable())
    {
      RAK12037_CO2 = airSensor.getCO2();
      RAK12037_temp = airSensor.getTemperature();
      RAK12037_humi = airSensor.getHumidity();
      if (DEBUG) Serial.printf("RAK12037: Temp = %3.1f °C, Humi = %3.0f %%rH, CO2 = %d ppm\n", RAK12037_temp, RAK12037_humi, RAK12037_CO2);
    }
    else
      if (DEBUG) Serial.println("RAK12037 - Waiting for new data");
  }
}
