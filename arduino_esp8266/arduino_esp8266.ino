/*
 Basic ESP8266 MQTT json example

 Demonstrates sending and receiving JSON over MQTT to the ESP8266.
 In this case, we will use React.js to control the brightness of
 the ESP8266 onboard led.

*/

#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int BUFFER_SIZE = JSON_OBJECT_SIZE(20);

byte brightness = 255;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  analogWriteRange(255);

  setup_wifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
   
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf(" ESP8266 Chip id = %08X\n", ESP.getChipId());
}

void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<BUFFER_SIZE> doc;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  
  Serial.println(message);

  DeserializationError error = deserializeJson(doc, message);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  
  JsonObject root = doc.as<JsonObject>();

  if (root.containsKey("brightness")) {
    byte newBrightness = constrain(root[String("brightness")], 0, 255);
    analogWrite(LED_PIN, (255 - newBrightness));
    brightness = newBrightness;
    Serial.print("Brightness: ");
    Serial.println(brightness);
  }

  if (root.containsKey("getState")) {
    sendSettings();
  }
}

void sendSettings() {
  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  
  root["brightness"] = brightness;

  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  
  char buffer[bufferSize];
  
  serializeJson(root, buffer);
  
  client.publish(MQTT_TOPIC_OUT, buffer, true);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
      client.subscribe(MQTT_TOPIC_OUT);
      client.subscribe(MQTT_TOPIC_IN);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
