#include <PersWiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266SSDP.h>
#include <PubSubClient.h>
#include <SPIFFSReadServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include "Wemos_Mini_OLED.h"

#define DEVICE_NAME "ESP8266 WiFi"

SPIFFSReadServer server(80);
DNSServer dnsServer;
WiFiClient espClient;
PubSubClient client(espClient);
PersWiFiManager persWM(server, dnsServer);

Wemos_Mini_OLED display(0);

const char* mqtt_server = "XXX.cloudmqtt.com";  // Your MQTT server
const char* user = "XXXXXXXXXXX";               // Your MQTT yourname
const char* pass = "XXXXXXXXXXXXXX";            // Your MQTT password

bool internet = false;
String s;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Loading..");
  display.display();
  //allows serving of files from SPIFFS
  SPIFFS.begin();
  //sets network name for AP mode
  persWM.setApCredentials(DEVICE_NAME);
  //persWM.setApCredentials(DEVICE_NAME, "password"); optional WiFi AP password
  persWM.onConnect([]() {
    client.setServer(mqtt_server, 16390);
    client.setCallback(callback);
    internet = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Ready :)");
    display.display();
  });
  persWM.onAp([]() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("setup\ncomplete!");
    display.println("Connect to\n\"ESP8266\nWiFi\"");
    display.display();
  });
  persWM.begin();
  server.on("/serial", []() {
    //build json object of program data
    StaticJsonDocument<500> json;
    long CurrentTime =  millis();
    int sc = CurrentTime / 1000;
    int mn = sc / 60;
    int hr = mn / 60;
    char ch[20];
    sprintf (ch, "%02d", sc % 60);
    json["sec"] = ch;
    sprintf (ch, "%02d", mn % 60);
    json["min"] = ch;
    sprintf (ch, "%02d", hr % 60);
    json["hr"] = ch;
    json["s"] = s;
    s = "";
    char jsonchar[500];
    serializeJson(json, jsonchar);  //print to char array, takes more memory but sends in one piece
    server.send(200, "application/json", jsonchar);

  }); //server.on serial

  //SSDP makes device visible on windows network
  server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(DEVICE_NAME);
  SSDP.setModelName("ESP8266 WiFi device");
  SSDP.setModelURL("/");
  SSDP.setManufacturer("HA4ever ^_*");
  SSDP.setManufacturerURL("https://github.com/HA4ever37");
  SSDP.setSerialNumber("No1");
  SSDP.setModelNumber("1");
  SSDP.begin();
  SSDP.setDeviceType("upnp:rootdevice");

  server.begin();
  s += ("<br>Setup complete.");
} //void setup

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  if (internet)
    if (!client.connected())
      reconnect();
  client.loop();
} //void loop

void callback(char* topic, byte* payload, unsigned int length) {
  display.clearDisplay();
  display.setCursor(0, 0);
  s += ("<br>Message arrived [");
  s += (topic);
  s += ("]: ");
  for (int i = 0; i < length; i++) {
    s += ((char)payload[i]);
  }
  if ((char)payload[0] == '*') {
    msgConfrim("Blink led");
    for (byte b = 0; b < 25; b++) { //blink the led for 5 seconds
      client.loop();
      server.handleClient();
      digitalWrite(BUILTIN_LED, LOW);
      delay(100);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(100);
    }
  }
  else if ((char)payload[0] == '#') {
    WiFiClient client;
    HTTPClient http;
    String payload = "Error!";
    if (http.begin(client, "http://api.ipify.org")) {
      if (http.GET() > 0)
        payload = http.getString();
    }
    char ip[16];
    payload.toCharArray(ip, 16);
    msgConfrim(ip);
  }
  else if ((char)payload[0] == '0') {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    msgConfrim("Led is OFF");
  }
  else if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    msgConfrim("Led is ON");
  }
  else if ((char)payload[0] == '2') {
    display.setTextSize(2);
    msgConfrim("Set large font");
  }
  else if ((char)payload[0] == '3') {
    display.setTextSize(1);
    msgConfrim("Set small font");
  }
  else if ((char)payload[0] == ':') {
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    String oTime = "Online time: ";
    char ch[4];
    sprintf (ch, "%02d", hr % 60);
    oTime += ch;
    oTime += ':';
    sprintf (ch, "%02d", min % 60);
    oTime += ch;
    oTime += ':';
    sprintf (ch, "%02d", sec % 60);
    oTime += ch;
    msgConfrim(strdup(oTime.c_str()));
  }
  else {
    display.clearDisplay();
    display.setCursor(0, 0);
    for (int i = 0; i < length; i++) {
      display.print((char)payload[i]);
      display.display();
      yield();
    }
    msgConfrim("New message is received!");
  }
}

void msgConfrim(char* msg) {
  s += ("<br>Publish message: ");
  s += (msg);
  client.publish("outTopic", msg, 1);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    s += ("<br>Attempting MQTT connection...");
    // Create a random client ID
    String clientId = DEVICE_NAME;
    // Attempt to connect
    if (client.connect(clientId.c_str(), user, pass)) {
      s += ("<br>Connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "ESP8266 WiFi is on!");
      client.subscribe("inTopic");
    }
    else {
      display.clearDisplay();
      display.setCursor(0, 0);
      s += ("Failed, rc=");
      display.print("Failed, rc=");
      s += (client.state());
      display.println(client.state());
      s += ("<br>Try again in 5 seconds");
      display.println("Trying\nagain..");
      display.display();
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
