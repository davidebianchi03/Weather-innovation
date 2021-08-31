#include "env_settings.h"//impostazioni locali
#include <ArduinoJson.h>//stringhe in formato json
/*Librerie per usare il wifi*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <SimpleCLI.h>

SimpleCLI cli;
Command set;

bool device_connected = false;

void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);
  connect_to_wifi();

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

long last_request_millis = 0;
bool first_request = true;
String survey_type = "";
int min_value = 0;
int max_value = 0;

void loop(){
   if (Serial.available()) {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        //Visualizzo l'input dell'utente
        Serial.print("# ");
        Serial.println(input);

        cli.parse(input);
    }

    if(device_connected){
      if(first_request || millis() - last_request_millis >= 60000){
        //Invio richiesta al server
        
      }
    }
    
}

bool connect_to_wifi() { //funzione per connettersi al wifi(restituisce true se riesce a collegarsi al wifi)
  const int wifi_connection_timeout = 5000;//timeout per la connessione al wifi in millisecondi
  WiFi.begin( get_ssid_from_eeprom(), get_password_from_eeprom());
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
    device_connected = true;
  }
  else{
    Serial.println("Error during connection");
    Serial.print("SSID: ");
    Serial.println(get_ssid_from_eeprom());
    Serial.print("PASSWORD: ");
    Serial.println(get_password_from_eeprom());
  }

  return connection_successful;
}

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
    EEPROM.commit();
  }

  
}

String read_string_from_eeprom(int offset, int lenght) {
  String string_read = "";

  for (int i = offset; i < offset + lenght; i++) {
    string_read += (char)EEPROM.read(i);
  }

  return string_read;
}

String get_last_survey_json_string(){
  String request_url = SERVER_PATH + "api/getlastsurvey";
  WiFiClient wifi_client;
  HTTPClient http;
  http.begin(wifi_client, request_url);
  StaticJsonDocument<200> doc;
  doc["device_id"] = DEVICE_ID;
  String http_request_body;
  serializeJson(doc,http_request_body);
  int http_response_code = http.POST(http_request_body);
  if(http_response_code > 0){
    String response = http.getString();
    Serial.println(http_response_code);
    Serial.println(response);
  }
  else {
    Serial.printf("Errore durante l'invio dei dati al server: %s\n", http.errorToString(http_response_code).c_str());
  }
  return "";
}
