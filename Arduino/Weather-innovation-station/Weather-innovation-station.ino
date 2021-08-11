#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <ArduinoJson.h>//stringhe in formato json
#include <HTTPClient.h>
#include <Wire.h>//i2c
#include <BMx280I2C.h>//sensore pressione atmosferica bm280
#include "env_settings.h"//impostazioni locali

#define BMX280_I2C_ADDRESS 0x76
//Sensore temperatura e umidità dht11
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

BMx280I2C bmx280(BMX280_I2C_ADDRESS);//sensore pressione
DHT dht(DHTPIN, DHTTYPE);//sensore umidità e temperatura
int survey_update_time = 2000;//tempo tra una rilevazione e la successiva in millisecondi

void setup() {
  Serial.begin(115200);
  Wire.begin();//inizializzazione bus i2c
  connect_to_wifi();//Connessione al wifi
  get_settings();//ottengo le impostazioni della stazione meteo dal server
  /*Inizializzazione sensore pressione atmosferica bmx280*/
  bmx280.begin();
  bmx280.resetToDefaults();
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  /*Inizializzazione sensore umidità e temperatura*/
  dht.begin();
}

void loop() {
  int atmospheric_pressure = get_atmospheric_pressure();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float longitude = 9.656;
  float latitude = 45.759;
  send_data_to_server(temperature, humidity, atmospheric_pressure,longitude, latitude);
  
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
  http.begin(url);
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

bool send_data_to_server(float temperature, float humidity, float pressure, float longitude, float latitude) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["latitude"] = latitude;
  doc["longitude"] = longitude;

  String url = SERVER_PATH + "/api/recivedata";
  HTTPClient http;
  http.begin(url);
  String http_json_data;
  serializeJson(doc, http_json_data);
  int http_response_code = http.POST(http_json_data);
  if (http_response_code > 0) {
    //String response = http.getString();
    Serial.println(http_response_code);
    //Serial.println(response);
  }
  else {
    Serial.printf("Errore durante l'invio dei dati al server: %s\n", http.errorToString(http_response_code).c_str());
  }
}

int get_atmospheric_pressure() { //funzione per ottenere il valore della pressione atmosferica dal sensore di umidità e temperatura bm280
  bmx280.measure();

  do {
    delay(100);
  } while (!bmx280.hasValue());
  return bmx280.getPressure() / 100;
}
