#include "env_settings.h"//impostazioni locali
#include <ArduinoJson.h>//stringhe in formato json
/*Librerie per usare il wifi*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

void setup(){
  Serial.begin(115200);
  connect_to_wifi();
}

void loop(){
  delay(60000);
}

bool connect_to_wifi() { //funzione per connettersi al wifi(restituisce true se riesce a collegarsi al wifi)
  const int wifi_connection_timeout = 5000;//timeout per la connessione al wifi in millisecondi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wifi.");
  long connection_attempt_start = millis();
  bool connection_successful = false;

  while (WiFi.status() != WL_CONNECTED && wifi_connection_timeout > millis() - connection_attempt_start) {
    Serial.print(".");
    delay(100);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    connection_successful = true;
    Serial.print("Connected to wifi with IP address: ");
    Serial.println(WiFi.localIP());
  }

  return connection_successful;
}
