// RAK1902

boolean initRAK1902(void)
{
  if (BARO.begin()) RAK1902_ready = true;
  else Serial.println("RAK1902 not detected");

  return RAK1902_ready;
}

void readRAK1902(void)
{
  RAK1902_temp = BARO.readTemperature();
  RAK1902_press = BARO.readPressure();
   
  if (DEBUG) Serial.printf("RAK1902 : Temp = %3.1f °C, Humi = %3.0f kPa\n", RAK1902_temp, RAK1902_press);
}
