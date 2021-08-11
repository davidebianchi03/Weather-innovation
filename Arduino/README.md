# USING ARDUINO SKETCH
Per poter utilizzare lo sketch di arduino è necessario creare un file chiamato "env_settings.h" nella cartella "Weather-innovation-station" contenente l'ssid e la password del wifi scritti come nel seguente esempio
```
#include "Arduino.h"

#define WIFI_SSID "zyxel"
#define WIFI_PASSWORD "mauro1969"
const String SERVER_PATH = "http://192.168.1.49:8000";
#define DEVICE_ID a0001
```
