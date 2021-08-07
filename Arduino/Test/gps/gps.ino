/*
  MQTT and AskSensors IoT Platform
  Description: ESP32 publishes NEO-6M GPS position to AskSensors using MQTT
  Author: https://asksensors.com, 2020
  github: https://github.com/asksensors
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//TODO: ESP32 MQTT user config
const char* ssid = ".................."; // Wifi SSID
const char* password = ".................."; // Wifi Password
const char* username = "................."; // my AskSensors username
const char* pubTopic = "publish/..../....."; // publish/username/apiKeyIn
const unsigned int writeInterval = 25000; // write interval (in ms)

static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;
//AskSensors MQTT config
const char* mqtt_server = "mqtt.asksensors.com";
unsigned int mqtt_port = 1883;

// objects
WiFiClient askClient;
PubSubClient client(askClient);
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

// setup
void setup() {
  Serial.begin(115200);
  // GPS baud rate
  ss.begin(GPSBaud);

}
// loop
void loop() {

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

// GPS displayInfo
void displayInfo() {

  if (gps.location.isValid()) {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);


  }
}
