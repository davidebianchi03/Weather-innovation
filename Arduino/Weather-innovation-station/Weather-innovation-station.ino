#include <ArduinoJson.h>//stringhe in formato json
#include <HTTPClient.h>
#include <Wire.h>//i2c
#include <BMx280I2C.h>//sensore pressione atmosferica bm280
#include <WiFi.h>
#include "env_settings.h"//impostazioni locali

#define BMX280_I2C_ADDRESS 0x76

BMx280I2C bmx280(BMX280_I2C_ADDRESS);

int survey_update_time = 2000;//tempo tra una rilevazione e la successiva in millisecondi

void setup() {
  Serial.begin(115200);
  connect_to_wifi();//Connessione al wifi
  get_settings();//ottengo le impostazioni della stazione meteo dal server
  Serial.println(survey_update_time);
}

void loop() {

  delay(survey_update_time);
}

bool connect_to_wifi() { //funzione per connettersi al wifi(restituisce true se riesce a collegarsi al wifi)
  const int wifi_connection_timeout = 5000;//timeout per la connessione al wifi in millisecondi
  WiFi.mode(WIFI_STA);
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

void disconnect_from_wifi() { //procedura per disconnettersi dal wifi
  WiFi.disconnect();
  Serial.println("Disconnected from wifi");
}

void get_settings() {
  String url = SERVER_PATH + "/api/getsettings";
  HTTPClient http;
  http.begin(url.c_str());
  int http_response_code = http.GET();

  if (http_response_code > 0) {
    String response_payload = http.getString();
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response_payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    survey_update_time = doc["Update_time"];
  }
  else {
    Serial.print("Error during get settings, error code: ");
    Serial.println(http_response_code);
  }
  http.end();
}
