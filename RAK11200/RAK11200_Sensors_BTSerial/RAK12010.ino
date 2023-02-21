// RAK12010

boolean initRAK12010()
{
  if (veml.begin()) 
  {
    RAK12010_ready = true;
  } 
  else Serial.println("RAK12010 not detected");

  return RAK12010_ready;
}

void readRAK12010(void)
{
  RAK12010_lux = veml.readLux(VEML_LUX_AUTO);
  if (DEBUG) Serial.printf("RAK12010: Lux  = %3.1f Lux\n", RAK12010_lux);
} 