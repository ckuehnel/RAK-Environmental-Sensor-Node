/*
 * File: RAK3372_LoRaWAN.ino
 * 
 * Test of LoRaWAN connction to TTS(CE) for RAK3372 (RUI)
 * based on Smart_Farm Example but w/ simulated measuring values
 *
 * 2023-01-26 Claus Kühnel indo2ckuehnel.ch
*/ 
 
#define PERIOD   (60000)
/*************************************

   LoRaWAN band setting:
     RAK_REGION_EU433
     RAK_REGION_CN470
     RAK_REGION_RU864
     RAK_REGION_IN865
     RAK_REGION_EU868
     RAK_REGION_US915
     RAK_REGION_AU915
     RAK_REGION_KR920
     RAK_REGION_AS923

 *************************************/
#define BAND     (RAK_REGION_EU868)
#define DEVEUI   {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x9C, 0xAC}
#define APPEUI   {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x09, 0x60, 0x01}
#define APPKEY   {0x90, 0xB8, 0x37, 0x99, 0xBE, 0xC9, 0x8F, 0x00, 0xCF, 0x02, 0x50, 0xF3, 0xEA, 0x7A, 0xDD, 0x3E}


/** Packet buffer for sending */
uint8_t collected_data[64] = { 0 };

void recvCallback(SERVICE_LORA_RECEIVE_T * data)
{
  if (data->BufferSize > 0) {
    Serial.println("Something received!");
    for (int i = 0; i < data->BufferSize; i++) {
      Serial.printf("%x", data->Buffer[i]);
    }
    Serial.print("\r\n");
  }
}

void joinCallback(int32_t status)
{
  Serial.printf("Join status: %d\r\n", status);
}

void sendCallback(int32_t status)
{
  if (status == 0) {
    Serial.println("Successfully sent");
  } else {
    Serial.println("Sending failed");
  }
}

void setup()
{
  Serial.begin(115200, RAK_AT_MODE);

  Serial.println("RAK Environmental Sensor Node");
  Serial.println("-----------------------------");

  // OTAA Device EUI MSB first
  uint8_t node_device_eui[8] = DEVEUI;
  // OTAA Application EUI MSB first
  uint8_t node_app_eui[8] = APPEUI;
  // OTAA Application Key MSB first
  uint8_t node_app_key[16] = APPKEY;

  if (!api.lorawan.appeui.set(node_app_eui, 8)) {
    Serial.printf
	("RAK Env Sensor Node - set application EUI is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.appkey.set(node_app_key, 16)) {
    Serial.printf
	("RAK Env Sensor Node - set application key is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.deui.set(node_device_eui, 8)) {
    Serial.printf
	("RAK Env Sensor Node - set device EUI is incorrect! \r\n");
    return;
  }

  if (!api.lorawan.band.set(BAND)) {
    Serial.printf("RAK Env Sensor Node - set band is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.deviceClass.set(RAK_LORA_CLASS_A)) {
    Serial.printf
	("RAK Env Sensor Node - set device class is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.njm.set(RAK_LORA_OTAA))	// Set the network join mode to OTAA
  {
    Serial.printf
	("RAK Env Sensor Node - set network join mode is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.join())	// Join to Gateway
  {
    Serial.printf("RAK Env Sensor Node - join fail! \r\n");
    return;
  }

  Serial.println("++++++++++++++++++++++++++");
  Serial.println("RUI3 Environment Sensing");
  Serial.println("++++++++++++++++++++++++++");

  /** Wait for Join success */
  while (api.lorawan.njs.get() == 0) {
    Serial.print("Wait for LoRaWAN join...");
    api.lorawan.join();
    delay(10000);
  }

  if (!api.lorawan.adr.set(true)) {
    Serial.printf
	("RAK Env Sensor Node - set adaptive data rate is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.rety.set(1)) {
    Serial.printf
	("RAK Env Sensor Node - set retry times is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.cfm.set(1)) {
    Serial.printf
	("RAK Env Sensor Node - set confirm mode is incorrect! \r\n");
    return;
  }

  /** Check LoRaWan Status*/
  Serial.printf("Duty cycle is %s\r\n", api.lorawan.dcs.get()? "ON" : "OFF");	// Check Duty Cycle status
  Serial.printf("Packet is %s\r\n", api.lorawan.cfm.get()? "CONFIRMED" : "UNCONFIRMED");	// Check Confirm status
  uint8_t assigned_dev_addr[4] = { 0 };
  api.lorawan.daddr.get(assigned_dev_addr, 4);
  Serial.printf("Device Address is %02X%02X%02X%02X\r\n", assigned_dev_addr[0], assigned_dev_addr[1], assigned_dev_addr[2], assigned_dev_addr[3]);	// Check Device Address
  Serial.printf("Uplink period is %ums\r\n", PERIOD);
  Serial.println("");
  api.lorawan.registerRecvCallback(recvCallback);
  api.lorawan.registerJoinCallback(joinCallback);
  api.lorawan.registerSendCallback(sendCallback);
}

void uplink_routine()
{
  // no sensors, simulation only
  float temp_f = 22.2 + random(10)/10.; 
  float humid_f = 55.0 + random(20)/10.;
   
  uint16_t t = (uint16_t) (temp_f * 10.0);
  uint16_t h = (uint16_t) (humid_f * 2);

  float bat_f = (api.system.bat.get());
  uint16_t bat = (uint16_t) (bat_f * 100);
  Serial.printf("T %.2f H %.2f B %.3f\r\n", temp_f, humid_f, bat_f);

  /** Cayenne Low Power Payload */
  uint8_t data_len = 0;
  collected_data[data_len++] = 0x01;	//Data Channel: 1
  collected_data[data_len++] = 0x67;	//Type: Temperature Sensor
  collected_data[data_len++] = (uint8_t) (t >> 8);
  collected_data[data_len++] = (uint8_t) t;
  collected_data[data_len++] = 0x02;	//Data Channel: 2
  collected_data[data_len++] = 0x68;	//Type: Humidity Sensor
  collected_data[data_len++] = (uint8_t) h;
//  collected_data[data_len++] = 0x03;	//Data Channel: 3
//  collected_data[data_len++] = 0x73;	//Type: Barometer
//  collected_data[data_len++] = (uint8_t) ((pre & 0x0000FF00) >> 8);
//  collected_data[data_len++] = (uint8_t) (pre & 0x000000FF);
  collected_data[data_len++] = 0x04;  //Data Channel: 4
  collected_data[data_len++] = 0x02;  //Type: Analog Input
  collected_data[data_len++] = (uint8_t)(bat >> 8);
  collected_data[data_len++] = (uint8_t)bat;

  Serial.println("Data Packet:");
  for (int i = 0; i < data_len; i++) {
    Serial.printf("0x%02X ", collected_data[i]);
  }
  Serial.println("");

  /** Send the data package */
  if (api.lorawan.send(data_len, (uint8_t *) & collected_data, 2, true, 1)) {
    Serial.println("Sending is requested");
  } else {
    Serial.println("Sending failed");
  }
}

void loop()
{
  uplink_routine();

  Serial.printf("Try sleep %ums..\r\n", PERIOD);
  api.system.sleep.all(PERIOD);
  Serial.println("Wakeup..\r\n");
}
