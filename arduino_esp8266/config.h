/*
 MQTT connection settings

 This sketch demonstrates JSON over MQTT
 
 MQTT / TCP Broker: mqtt://broker.mqtt-dashboard.com:1883
 Websocket Broker: ws://broker.mqttdashboard.com:8000/mqtt
 
*/

// LED
#define LED_PIN LED_BUILTIN

// WiFi
#define WIFI_SSID "..." // Define wifi hostname
#define WIFI_PASS "..." // Define wifi password

// MQTT
#define MQTT_HOST "broker.mqtt-dashboard.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP8266"

// MQTT Topics
#define MQTT_TOPIC_OUT "esp/json"
#define MQTT_TOPIC_IN "esp/json/set"

#define DEBUG true
