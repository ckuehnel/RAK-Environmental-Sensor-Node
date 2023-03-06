# RAK Environmental Sensor Nodes

The first RAK Environmental Sensor node is based on the following WisBlock Modules, the second by RAK5005-0, RAK11310 & RAK1920. 

![image](https://user-images.githubusercontent.com/195788/216775481-fd42f565-26e3-4ed2-b131-239d62add14a.png)

There are three projects in separate folders
- RAK11200
- RAK3372 LoRaWAN
- RAK1300 LoRaWAN

### RAK11200
RAK11200 (ESP32) is base for this node. Measured values will be prepared and send by WiFi to the internet. 
Visualization of data by MQTT and InfluxDB.

### RAK3372 LoRaWAN
RAK3372 (STM32) is base for this node. Measured values will be packed as Cayenne Low Power Payload (LPP) which provides a convenient and easy way to send data to TTS(CE).

### Sensirion SEN55 & RAK1300 LoRaWAN
Data aquired by Sensirion SEN55 and packed as Cayenne Low Power Payload (LPP) is sent via TTN to Datacake for visualization.

You will find further information to LoRaWAN-nodes and TTS(CE) in my books:

![LoRa_Cover_D](https://user-images.githubusercontent.com/195788/214926264-a648e74a-572d-487c-996b-6d29f237d446.png)
![LoRa_Cover_EN](https://user-images.githubusercontent.com/195788/214926215-85af6cda-57ce-466c-a75e-1afc3aebef99.png)

You will find them @  https://www.elektor.de/lorawan-knoten-im-iot resp. https://www.elektor.com/develop-and-operate-your-lorawan-iot-nodes

