# D1Mini_OLED_MQTT
Minimal fully automated MQTT client using ESP8266 (D1 mini preferably) and Wemos Oled 0.66 

Features:
* WiFi Manager to scan and connect to your network (no need for hard coded credentials). If the WiFi AP is not available then the Wifi Manager will create its own AP so the user can it for enter new credentials
* Ability to Subscribe, Publish, and print MQTT messages on screen
* Autoconnect when disconnected or if there is a network error
* Ability to print animated messages on screen and accept commands from an MQTT subscribed topic 
* Active http server to show debugging messages and MQTT messages instead of using Serial monitor
* Various usfull contol commands (Print IP adrress, running time, led control, change font size, etc)
* SPIFFS read/write abitily so you can upload HTML files to your ESP8266 easily (e.g. change the style or clolors of WiFi Manager page)
* Compressed gzip HTML files to save space
* DNS Server for easy initial setup from your pc or phone
* SSDP Server so your ESP8266 can be seen as wifi device when connected to your home/office wifi network


Future features:
* Ask for a username and password to access debugging messages' page
* Other suggestions? 

Required external libraries (can be found from Library Manager of the Arduino IDE):
* ArduinoJson
* Adafruit_GFX
* PersWiFiManager v4 (NOT v5)
* PubSubClient
* SPIFFSReadServer

Needed hardware:
* Generic ESP8366, D1 mini preffered
* I2C 0.66 OLED screen (Wemos d1 mini Oled)


IMPORTANT NOTES!
* Don't forget to upload the content of "data" folder to your ESP8266 using "ESP8266 Sketch Data Upload" tool
* List of MQTT commands can be found in "Commands.txt"
