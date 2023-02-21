/*
 * File: RAK11200_BTSensors_Serial.ino
 * 
 * Environmwental Sensor based on
 * - RAK19007 WisBlock Base Board 2nd Gen
 * - RAK11200 is a WisBlock Core module based on Espressif ESP32-WROVER
 * - RAK1901 WisBlock Sensor based on Sensirion SHTC3 Temperature and Humidity Sensor
 * - RAK1902 WisBlock Sensor based on STMicroelectronics LPS22HB Barometric Pressure Sensor 
 * - RAK12010 WisBlock Light Sensor based on Vishay VEML7700
 * - RAK12047 WisBlock VOC Sensor based on MOx-based Sensirion Gas Sensor SGP40
 * - RAK12037 WisBlock CO2 Sensor based Sensirion SCD30
 * - RAK12039 Wisblock Particle Matter Sensor based Plantower PMSA003I
 * reads data from sensors and prepares it for serial output.
 * 
 * 2023-02-10 Claus Kuehnel info@ckuehnel.ch
 */

#include <Wire.h>
#include "SHTSensor.h"                      // http://librarymanager/All#arduino-sht By:Johannes Winkelmann
#include <Arduino_LPS22HB.h>                // http://librarymanager/All#Arduino_LPS22HB
#include "Adafruit_VEML7700.h"              // http://librarymanager/All#Adafruit_VEML7700
#include "SparkFun_SGP40_Arduino_Library.h" // http://librarymanager/All#SparkFun_SGP40
#include "SparkFun_SCD30_Arduino_Library.h" // http://librarymanager/All#SparkFun_SCD30
#include "RAK12039_PMSA003I.h"              // https://github.com/RAKWireless/RAK12039-PMSA003I   
#include "esp_adc_cal.h"
#include "arduino_secrets.h"
#include <WiFi.h>
// always include thingspeak header file after other header files and custom macros
#include <ThingSpeak.h>                     // https://github.com/mathworks/thingspeak-arduino

#define DEBUG 1

WiFiClient  client;

SHTSensor sht;                                // RAK1901 - SHTC3
Adafruit_VEML7700 veml = Adafruit_VEML7700(); // RAK12010 - VEML7700
SGP40 mySGP40;                                // RAK12047 - SGP40
SCD30 airSensor;                              // RAK12037 - SCD30
RAK_PMSA003I PMSA003I;                        // RAK12049 - PMSA003I

boolean RAK1901_ready = false;   // true, if RAK1901 was detected
boolean RAK1902_ready = false;   // true, if RAK1902 was detected
boolean RAK12010_ready = false;  // true, if RAK12010 was detected
boolean RAK12047_ready = false;  // true, if RAK12047 was detected
boolean RAK12037_ready = false;  // true, if RAK12037 was detected
boolean RAK12039_ready = false;  // true, if RAK12039 was detected


float RAK1901_temp, RAK1901_humi;   // Temp & Humi of SHTC3 (RAK1901)
float RAK1902_temp, RAK1902_press;  // Temp & Pressure of LPS22HB (RAK1902)
float RAK12010_lux;                 // Lux of VEML7700 (RAK12010)
int32_t RAK12047_iVOC;              // VOC Index of SGP40 (RAK12047)
float RAK12037_temp, RAK12037_humi; // Temp & Humi of SCD30 (RAK12037
uint16_t RAK12037_CO2;              // CO2 of SCD30 (RAK12037)
uint16_t RAK12039_PM10, RAK12039_PM25, RAK12039_PM100; // Particle matters (RAK12039)
uint16_t batteryLevel = 100;

#define PIN_VBAT WB_A0
uint32_t vbat_pin = PIN_VBAT;

float Voltage = 0.0;
uint32_t readADC_Cal(int ADC_Raw);

unsigned long previousMillis = 0;   // will store last time of output
const long interval =  60000;       // interval for output to Thingspeak in ms

void setup() 
{
  // Sensor power on.
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH); // set 3V3_S 
  delay(300);

  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
 
  Wire.begin();
  Serial.begin(115200);
  delay(1000); // wait for serial monitor
  Serial.println("RAK11200 Environmental Sensor Node");
  
  if (initRAK1901())  Serial.println("RAK1901 connected");
  if (initRAK1902())  Serial.println("RAK1902 connected"); 
  if (initRAK12010()) Serial.println("RAK12010 connected"); 
  if (initRAK12047()) Serial.println("RAK12047 connected");
  if (initRAK12037()) Serial.println("RAK12037 connected"); 
  if (initRAK12039()) Serial.println("RAK12039 connected");    

  if (DEBUG) print_adc_characteristics();

  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) 
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      while(WiFi.status() != WL_CONNECTED)
      {
        WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        Serial.print(".");
        delay(500);     
      } 
      Serial.println("\nConnected.");
    }
    if (RAK1901_ready)  readRAK1901();
    if (RAK1902_ready)  readRAK1902();
    if (RAK12010_ready) readRAK12010();
    if (RAK12047_ready) readRAK12047();
    if (RAK12037_ready) readRAK12037();
    if (RAK12039_ready) readRAK12039();

    Voltage = (readADC_Cal(analogRead(PIN_VBAT))) * 2;
    if (DEBUG) Serial.printf("Voltage : Vbat = %.0f mV\n", Voltage); // Print Voltage (in mV)

    dataOutput();  

    //delay(60000);    
  }
}
