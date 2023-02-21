// Data Output

void dataOutput(void)
{
  const char stars[] = "********************************************************\n";

  SerialBT.printf(stars);
  SerialBT.printf("RAK Environmental Sensors Measuring Results\n");
  if (RAK1901_ready)  SerialBT.printf("RAK1901 : Temp = %3.1f °C, Humi = %3.0f %%rH\n", RAK1901_temp, RAK1901_humi);
  if (RAK1902_ready)  SerialBT.printf("RAK1902 : Temp = %3.1f °C, Humi = %3.0f kPa\n", RAK1902_temp, RAK1902_press);
  if (RAK12010_ready) SerialBT.printf("RAK12010: Lux  = %3.1f Lux\n", RAK12010_lux);
  if (RAK12047_ready) SerialBT.printf("RAK12047: iVOC = %3d\n", RAK12047_iVOC);
  if (RAK12037_ready) SerialBT.printf("RAK12037: Temp = %3.1f °C, Humi = %3.0f %%rH, CO2 = %d ppm\n",RAK12037_temp, RAK12037_humi, RAK12037_CO2);
  if (RAK12039_ready) SerialBT.printf("RAK12039: PM1.0 = %3d µg/𝑚3, PM2.5 = %3d µg/𝑚3, PM10 = %3d µg/𝑚3\n", RAK12039_PM10, RAK12039_PM25, RAK12039_PM100); 
                      SerialBT.printf("Voltage : Vbat = %.0f mV\n", Voltage); // Print Voltage (in mV)   
  SerialBT.printf(stars); 
} 
  


