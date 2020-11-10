#include "wifi-config.h"

#include <DHTesp.h>
#include <ESP8266WiFi.h>

#define DHT11_PIN 2

DHTesp dht;

const char* ssid     = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.setup(4, DHTesp::DHT11); // Connect DHT sensor to GPIO 4

  Serial.println("Setup done");

}

void loop() {
  delay(dht.getMinimumSamplingPeriod());

  Serial.println(dht.getStatusString());
  Serial.print("Temperature: ");
  Serial.println(dht.getTemperature(), 1);
  Serial.print("Humidity: ");
  Serial.println(dht.getHumidity()), 1;

  delay(1000);
}