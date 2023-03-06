/*
 * File: RAK11300_SEN55_Test.ino
 * 
 * Environmwental Sensor based on
 * - RAK5005-0 WisBlock Base Board
 * - RAK11310 is a WisBlock Core module based on Raspberry Pi Pico
 * - SEN55 Environmental Sensor Module by Sensirion
 * reads data from sensors and prepares it for serial & LoRaWAN output.
 * 
 * 2023-03-03 Claus Kuehnel info@ckuehnel.ch
 * 
 * based on https://github.com/Sensirion/arduino-i2c-sen5x/blob/master/examples/exampleUsage/exampleUsage.ino
 * based on https://github.com/RAKWireless/WisBlock/blob/master/examples/RAK11300/communications/LoRa/LoRaWAN/LoRaWAN_OTAA_ABP/LoRaWAN_OTAA_ABP.ino
 */

#include <Arduino.h>
#include "LoRaWan-Arduino.h" //http://librarymanager/All#SX126x
#include <SensirionI2CSen5x.h>
#include <Wire.h>
#include <SPI.h>
#include <stdio.h>
#include <CayenneLPP.h>

// Forward declarations
static void lorawan_has_joined_handler(void);
static void lorawan_join_failed_handler(void);
static void lorawan_rx_handler(lmh_app_data_t *app_data);
static void lorawan_confirm_class_handler(DeviceClass_t Class);
static void send_lora_frame(void);
void lorawan_unconf_finished(void);
void lorawan_conf_finished(bool result);
void buildCayenneLPP(void);

//#define LORAWAN_APP_INTERVAL 20000  // Defines the application data transmission interval. 20 s, value in [ms].
#define LORAWAN_APP_INTERVAL 5*60000  // Defines the application data transmission interval. According to the Fair Use Policy 5 min, value in [ms].

SensirionI2CSen5x sen5x;
CayenneLPP lpp(51);

// SEN55 measuring results
float massConcentrationPm1p0;
float massConcentrationPm2p5;
float massConcentrationPm4p0;
float massConcentrationPm10p0;
float ambientHumidity;
float ambientTemperature;
float vocIndex;
float noxIndex;
  
static uint32_t count = 0;
static uint32_t count_fail = 0;

bool send_now = false;

void setup() 
{
  // Sensor power on.
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH); // set 3V3_S 
  delay(300);

  // RAK1920 5V DC power on.
  pinMode(WB_IO5, OUTPUT);
  digitalWrite(WB_IO5, HIGH); // set 5 V 
  delay(300);
  
  Serial.begin(115200);
  while (!Serial) delay(100);
  Serial.println("Start Initialization...");

  if (initSEN55())  Serial.println("SEN55 connected.");  
  if (initLoRaWAN()) Serial.println("LoRaWAN initialized.");
  
}

unsigned long time_now = 0;

void loop() 
{
  readSEN55();

  if(millis() >= time_now + LORAWAN_APP_INTERVAL)
  {
    time_now += LORAWAN_APP_INTERVAL;
    buildCayenneLPP();
    Serial.println("Sending frame now...");
    send_lora_frame();
  }

}
