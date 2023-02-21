// Data Output

void dataOutput(void)
{
  // set the Thingspeak fields with the values
  ThingSpeak.setField(1, RAK1901_temp);
  ThingSpeak.setField(2, RAK1901_humi);
  ThingSpeak.setField(3, RAK1902_press);
  ThingSpeak.setField(4, RAK12010_lux);
  ThingSpeak.setField(5, RAK12047_iVOC);
  ThingSpeak.setField(6, RAK12037_CO2);

  String myStatus = "Air Quality ";
  if ((RAK12047_iVOC > 150) || (RAK12037_CO2 > 1000))
    myStatus += String("bad - Ventilation required");
  else
    myStatus += String("ok");

  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200) Serial.println("Channel update successful.");
  else         Serial.println("Problem updating channel. HTTP error code " + String(x));
} 
  
