# D1Mini_OLED_MQTT
Minimal MQTT client using ESP8266 (D1 mini) with Wemos Oled 0.66 

Features:
* Wifi Manager to scan and connect to your wifi (no need for hard coded credentials)
* Ability to Subscribe and Publish MQTT messages
* Ability to print messages on screen or accept commands from an MQTT subscribed topic 
* Active http server to show debugging messages instead of using Serial monitor

Future features:
* Ask for a username and password to access debugging messages page


Required external libraries (can be found from Library Manager of the Arduino IDE):
* ArduinoJson v5 NOT v6 beta
* Adafruit_GFX
* PersWiFiManager
* PubSubClient
* SPIFFSReadServer


Needed hardware:
* Generic ESP8366, D1 mini preffered
* I2C 0.66 OLED screen (Wemos d1 mini Oled)
