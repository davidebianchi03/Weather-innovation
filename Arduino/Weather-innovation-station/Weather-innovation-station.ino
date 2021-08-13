#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <ArduinoJson.h>//stringhe in formato json
#include <HTTPClient.h>
#include <Wire.h>//i2c
#include <BMx280I2C.h>//sensore pressione atmosferica bm280
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "env_settings.h"//impostazioni locali

#define BMX280_I2C_ADDRESS 0x76
//Sensore temperatura e umidità dht11
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

#define LED 18

BMx280I2C bmx280(BMX280_I2C_ADDRESS);//sensore pressione
DHT dht(DHTPIN, DHTTYPE);//sensore umidità e temperatura
int survey_update_time = 2000;//tempo tra una rilevazione e la successiva in millisecondi

TinyGPS gps;//GPS
SoftwareSerial ss(16, 17); //Porta seriale a cui è connesso il gps

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
  //Inizializzazione del GPS
  ss.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);
}

long last_survey_time = 0;
float longitude = 0;
float latitude = 0;
bool gps_valid_position = false;
void loop() {

  if (millis() - last_survey_time >= survey_update_time && gps_valid_position) { //Se è trascorso il tempo prefissato a partire dall'ultima rilevazione invio i dati
    int atmospheric_pressure = get_atmospheric_pressure();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    send_data_to_server(temperature, humidity, atmospheric_pressure, longitude, latitude);
  }

  //Rilevazione GPS
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    latitude = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
    Serial.print(latitude);
    Serial.print(" LON=");
    longitude = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
    Serial.print(longitude);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    gps_valid_position = true;
    digitalWrite(LED,LOW);
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
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
  doc["device_id"] = DEVICE_ID;

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
