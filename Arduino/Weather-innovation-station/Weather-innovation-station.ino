#include <SimpleCLI.h>
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
#include <EEPROM.h>

#define BMX280_I2C_ADDRESS 0x76
//Sensore temperatura e umidità dht11
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

#define LED 18

BMx280I2C bmx280(BMX280_I2C_ADDRESS);//sensore pressione
DHT dht(DHTPIN, DHTTYPE);//sensore umidità e temperatura
int survey_update_time = 300000;//tempo tra una rilevazione e la successiva in millisecondi

TinyGPS gps;//GPS
SoftwareSerial ss(16, 17); //Porta seriale a cui è connesso il gps

SimpleCLI cli;
Command set;

//lunghezza stringa ssid in pos = 0, inizio stringa ssid in pos = 1
String get_ssid_from_eeprom() {
  const int ssid_lenght_position = 0;
  const int ssid_offset = 1;

  int ssid_lenght = EEPROM.read(ssid_lenght_position);
  //Serial.println(ssid_lenght);
  String ssid_read = read_string_from_eeprom(ssid_offset, ssid_lenght);
  //Serial.println(ssid_read);
  return ssid_read;
}

void write_ssid_in_eeprom(String ssid) {
  const int ssid_lenght_position = 0;
  const int ssid_offset = 1;

  EEPROM.write(ssid_lenght_position, ssid.length()); //scrivo nella cella 0 la lunghezza della stringa dell'ssid
  EEPROM.commit();
  write_string_in_eeprom(ssid, ssid_offset);
  //Serial.println("SSID saved in flash memory");
}

String get_password_from_eeprom(){
  const int password_lenght_position = 32;
  const int password_offset = 33;

  int password_lenght = EEPROM.read(password_lenght_position);
  String password_read = read_string_from_eeprom(password_offset, password_lenght);
  return password_read;
}

void write_password_in_eeprom(String password){
  const int password_lenght_position = 32;
  const int password_offset = 33;

  EEPROM.write(password_lenght_position, password.length()); //scrivo nella cella 0 la lunghezza della stringa dell'ssid
  EEPROM.commit();
  write_string_in_eeprom(password, password_offset);
}

void write_string_in_eeprom(String string, int offset) {
  for (int i = offset; i < offset + string.length(); i++) {
    EEPROM.write(i, string[i - offset]);
  }

  EEPROM.commit();
}

String read_string_from_eeprom(int offset, int lenght) {
  String string_read = "";

  for (int i = offset; i < offset + lenght; i++) {
    string_read += (char)EEPROM.read(i);
  }

  return string_read;
}

void setup() {
  EEPROM.begin(512);
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
  digitalWrite(LED, HIGH);
  
  set = cli.addCmd("set", set_callback);
  cli.setOnError(errorCallback); // Set error Callback
  set.addArgument("parameter");
  set.addArgument("value");
}

void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void set_callback(cmd *c){
  Command cmd(c);

  Argument parameterArg = cmd.getArgument("parameter");
  Argument valueArg = cmd.getArgument("value");

  String parameter = parameterArg.getValue();
  String value = valueArg.getValue();

  
  if(parameter == "ssid"){
    write_ssid_in_eeprom(value);
    
  }
  else if(parameter == "password"){
    write_password_in_eeprom(value);
  }
}
long last_survey_time = 0;
float longitude = 0;
float latitude = 0;
bool gps_valid_position = false;
bool data_just_send = false;
void loop() {

  if (Serial.available()) {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        //Visualizzo l'input dell'utente
        /*Serial.print("# ");
        Serial.println(input);*/

        cli.parse(input);
    }

  if ((millis() - last_survey_time >= survey_update_time || !data_just_send) && gps_valid_position) { //Se è trascorso il tempo prefissato a partire dall'ultima rilevazione invio i dati
    int atmospheric_pressure = get_atmospheric_pressure();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    send_data_to_server(temperature, humidity, atmospheric_pressure, longitude, latitude);
    data_just_send = true;
    last_survey_time = millis();
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
    //Serial.print("LAT=");
    latitude = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
    //Serial.print(latitude);
    //Serial.print(" LON=");
    longitude = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
    /*Serial.print(longitude);
      Serial.print(" SAT=");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print(" PREC=");
      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());*/
    gps_valid_position = true;
    digitalWrite(LED, LOW);
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
  WiFi.begin(get_ssid_from_eeprom().c_str(), get_password_from_eeprom().c_str());
  //Serial.println(get_ssid_from_eeprom());
  //Serial.println(get_password_from_eeprom());
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
