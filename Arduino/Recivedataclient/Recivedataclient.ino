#include <HTTPClient.h>
#include "env_settings.h"//impostazioni locali
#include <WiFi.h>
#include <ArduinoJson.h>//stringhe in formato json
#include <EEPROM.h>
#include <SimpleCLI.h>
SimpleCLI cli;
Command set;
Command get;

bool device_connected;

#define LED 27

void setup() {
  pinMode(LED,OUTPUT);
  
  digitalWrite(LED,LOW);
  device_connected = false;
  Serial.begin(115200);
  EEPROM.begin(512);
  connect_to_wifi();

  set = cli.addCmd("set", set_callback);
  cli.setOnError(errorCallback); // Set error Callback
  set.addArgument("parameter");
  set.addArgument("value");
  get = cli.addCmd("get", get_callback);
  get.addArgument("parameter");
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

void set_callback(cmd *c) {
  Command cmd(c);

  Argument parameterArg = cmd.getArgument("parameter");
  Argument valueArg = cmd.getArgument("value");

  String parameter = parameterArg.getValue();
  String value = valueArg.getValue();


  if (parameter == "ssid") {
    write_ssid_in_eeprom(value);

  }
  else if (parameter == "password") {
    write_password_in_eeprom(value);
  }
  else if (parameter == "sensor") {
    write_sensor_in_eeprom(value);
  }
  else if (parameter == "maxvalue") {
    set_max_value(value.toInt());
  }
  else if (parameter == "minvalue") {
    set_min_value(value.toInt());
  }
  else if (parameter == "paireddeviceid") {
    write_paired_device_in_eeprom(value);
  }
  else if (parameter == "connection") {
    connect_to_wifi();
  }

}


void get_callback(cmd *c) {
  Command cmd(c);

  Argument parameterArg = cmd.getArgument("parameter");
  String parameter = parameterArg.getValue();

  if (parameter == "ssid") {
    Serial.println(get_ssid_from_eeprom());
  }
  else if (parameter == "password") {
    Serial.println(get_password_from_eeprom());
  }
  else if (parameter == "sensor") {
    Serial.println(get_sensor_from_eeprom());
  }
  else if (parameter == "maxvalue") {
    Serial.println(get_max_value());
  }
  else if (parameter == "minvalue") {
    Serial.println(get_min_value());
  }
  else if (parameter == "paireddeviceid") {
    Serial.println(get_paired_device_from_eeprom());
  }
  else if (parameter == "ip") {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Your Gateway is : ");
    Serial.println((WiFi.gatewayIP().toString().c_str()));

  }
  else if (parameter == "lastsurvey") {
    Serial.println(get_last_a_json_string());
  }

}

long last_request_millis = 0;
bool first_request = true;
String survey_type = "";
int min_value = 0;
int max_value = 0;

void loop() {
  if (Serial.available()) {
    // Read out string from the serial monitor
    String input = Serial.readStringUntil('\n');

    //Visualizzo l'input dell'utente
    Serial.print("# ");
    Serial.println(input);

    cli.parse(input);
  }
  if (WiFi.status() == WL_CONNECTED) {
    if (first_request || millis() - last_request_millis >= 60000) {
      first_request = false;
      last_request_millis = millis();
      //Invio richiesta al server
      String server_response = get_last_a_json_string();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, server_response);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      else {
        int last_survey_value = doc[get_sensor_from_eeprom()];
        Serial.println(last_survey_value);
        if(last_survey_value >= get_min_value() && last_survey_value<= get_max_value()){
          digitalWrite(LED,HIGH);
        }
        else{
          digitalWrite(LED,LOW);
        }
      }
    }
  }
  delay(500);
}

bool connect_to_wifi() { //funzione per connettersi al wifi(restituisce true se riesce a collegarsi al wifi)
  const int wifi_connection_timeout = 3000;//timeout per la connessione al wifi in millisecondi
  /*Serial.print("ssid: ");
    Serial.println(get_ssid_from_eeprom());
    Serial.print("password: ");
    Serial.println(get_password_from_eeprom());*/
  WiFi.mode(WIFI_STA);
  WiFi.begin( get_ssid_from_eeprom().c_str(), get_password_from_eeprom().c_str());
  Serial.print("Connecting to Wifi.");
  long connection_attempt_start = millis();
  bool connection_successful = false;

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 7) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    connection_successful = true;
    Serial.print("Connected to wifi with IP address: ");
    Serial.println(WiFi.localIP());
    device_connected = true;
    
  }
  else {
    Serial.println("Error during connection");
    Serial.print("SSID: ");
    Serial.println(get_ssid_from_eeprom());
    Serial.print("PASSWORD: ");
    Serial.println(get_password_from_eeprom());
    Serial.print("Error:");
    Serial.println(WiFi.status());
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

String get_password_from_eeprom() {
  const int password_lenght_position = 32;
  const int password_offset = 33;

  int password_lenght = EEPROM.read(password_lenght_position);
  String password_read = read_string_from_eeprom(password_offset, password_lenght);
  return password_read;
}

void write_password_in_eeprom(String password) {
  const int password_lenght_position = 32;
  const int password_offset = 33;

  EEPROM.write(password_lenght_position, password.length()); //scrivo nella cella 0 la lunghezza della stringa dell'ssid
  EEPROM.commit();
  write_string_in_eeprom(password, password_offset);
}

String get_sensor_from_eeprom() {
  const int sensor_lenght_position = 64;
  const int sensor_offset = 65;

  int sensor_lenght = EEPROM.read(sensor_lenght_position);
  String sensor_read = read_string_from_eeprom(sensor_offset, sensor_lenght);
  return sensor_read;
}

void write_sensor_in_eeprom(String sensor) {
  const int sensor_lenght_position = 64;
  const int sensor_offset = 65;

  EEPROM.write(sensor_lenght_position, sensor.length()); //scrivo nella cella 0 la lunghezza della stringa dell'ssid
  EEPROM.commit();
  write_string_in_eeprom(sensor, sensor_offset);
}

void set_max_value(int value) {
  const int max_value_position = 96;
  EEPROM.write(max_value_position, value);
  EEPROM.commit();
}

int get_max_value() {
  const int max_value_position = 96;
  return EEPROM.read(max_value_position);
}

void set_min_value(int value) {
  const int min_value_position = 97;
  EEPROM.write(min_value_position, value);
  EEPROM.commit();
}

int get_min_value() {
  const int min_value_position = 97;
  return EEPROM.read(min_value_position);
}

String get_paired_device_from_eeprom() {
  const int paired_device_lenght_position = 100;
  const int paired_device_offset = 101;

  int paired_device_lenght = EEPROM.read(paired_device_lenght_position);
  String paired_device_read = read_string_from_eeprom(paired_device_offset, paired_device_lenght);
  return paired_device_read;
}

void write_paired_device_in_eeprom(String paired_device) {
  const int paired_device_lenght_position = 100;
  const int paired_device_offset = 101;

  EEPROM.write(paired_device_lenght_position, paired_device.length()); //scrivo nella cella 0 la lunghezza della stringa dell'ssid
  EEPROM.commit();
  write_string_in_eeprom(paired_device, paired_device_offset);
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

String get_last_a_json_string() {
  String request_url = SERVER_PATH + "api/getlastsurvey";
  HTTPClient http;
  http.begin(request_url);
  StaticJsonDocument<200> doc;
  doc["device_id"] = get_paired_device_from_eeprom();
  String http_request_body;
  serializeJson(doc, http_request_body);
  int http_response_code = http.POST(http_request_body);
  if (http_response_code > 0) {
    String response = http.getString();
    Serial.println(http_response_code);
    //Serial.println(response);
    return response;
  }
  else {
    Serial.printf("Errore durante l'invio dei dati al server: %s\n", http.errorToString(http_response_code).c_str());
  }
  http.end();
  return "";
}
