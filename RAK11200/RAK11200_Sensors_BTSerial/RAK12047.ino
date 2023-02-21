// RAK12047

boolean initRAK12047(void)
{
  if (mySGP40.begin()) 
    RAK12047_ready = true;
  else 
    Serial.println("RAK12047 not detected.");
  
  return RAK12047_ready;
}

void readRAK12047(void)
{
  if(RAK12047_ready)
    RAK12047_iVOC = mySGP40.getVOCindex(RAK1901_humi, RAK1901_temp); 
  else 
    RAK12047_iVOC = mySGP40.getVOCindex(); //Get the VOC Index using the default RH (50%) and T (25C)

  if (DEBUG) Serial.printf("RAK12047: iVOC = %3d\n", RAK12047_iVOC);
}
