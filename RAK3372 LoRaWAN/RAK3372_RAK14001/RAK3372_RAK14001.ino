#include <Wire.h>
#include <NCP5623.h>  //http://librarymanager/All#NCP5623 By:RAKWireless

NCP5623 rgb;

void setup() 
{
  // enable RAK14001
  pinMode(WB_IO6, OUTPUT);
  digitalWrite(WB_IO6, HIGH);

  Serial.begin(115200);
  delay(2000);
  
  Wire.begin();
  Wire.setClock(100000);

  Serial.println("RAK14001 RGB LED Test");

  if (!rgb.begin())
  {
    Serial.println("RAK14001 not found on the I2C line");
    while (1);
  }
  else Serial.println("RAK14001 Found. Begining execution");

  // set the current output level max, the range is 1 to 31
  rgb.setCurrent(10);
}

void loop() 
{
  rgb.setColor(0,0,0);     // OFF
  delay(1000);
  rgb.setColor(255,0,0);    // RED
  delay(1000);
  rgb.setColor(0,255,0);    // GREEN
  delay(1000);
  rgb.setColor(0,0,255);    // BLUE
  delay(1000);
  rgb.setColor(255,255,0);  // YELLOW
  delay(1000);
  rgb.setColor(255,0,255);  // PURPLE
  delay(1000);
  rgb.setColor(0,255,255);  // CYAN
  delay(1000);
  rgb.setColor(255,255,255); // WHITE
  delay(1000);
}
