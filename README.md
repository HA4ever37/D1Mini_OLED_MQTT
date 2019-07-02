# D1Mini_OLED_MQTT
Minimal MQTT client using ESP8266 (D1 mini) and Wemos Oled 0.66 

Features:
* Wifi Manager to scan and connect to your wifi (no need for hard coded credentials)
* Ability to Subscribe and Publish MQTT messages
* Ability to print messages on screen and accept commands from an MQTT subscribed topic 
* Active http server to show debugging messages instead of using Serial monitor
* SPIFFS read/write abitily so you can upload files to your ESP8266 easily
* Compressed gzip HTML files to save space
* DNS Server for easy initial setup
* SSDP Server so your ESP8266 can be seen as wifi device when connected to your wifi network


Future features:
* Ask for a username and password to access debugging messages page
* Other suggestions? 

Required external libraries (can be found from Library Manager of the Arduino IDE):
* ArduinoJson v6
* Adafruit_GFX
* PersWiFiManager v4 NOT v5
* PubSubClient v4
* SPIFFSReadServer

Needed hardware:
* Generic ESP8366, D1 mini preffered
* I2C 0.66 OLED screen (Wemos d1 mini Oled)


IMPORTANT! don't forget to upload the content of "data" folder to your ESP8266 using "ESP8266 Sketch Data Upload" tool before you upload the sketch. 
MQTT commands can be found in "Commands.txt" 
