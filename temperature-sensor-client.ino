#include "wifi-config.h"
#include "httpclient-config.h"

#include <DHTesp.h>

#include <ESP8266WiFi.h>


DHTesp dht;

const char* ssid = STASSID;
const char* password = STAPSK;

const char* url = HTTPURL;

void setup() {
  Serial.begin(115200);
  Serial.println("");

  Serial.printf("[WiFi] Connect to %s\n", ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("[WiFi] Connected!");
  Serial.printf("[WiFi] IP address: %s\n", WiFi.localIP().toString().c_str());

  dht.setup(4, DHTesp::DHT11); // Connect DHT sensor to GPIO 4

  Serial.println("[Init] Setup done");

}

void loop() {
  delay(dht.getMinimumSamplingPeriod());

  const char* status = dht.getStatusString();
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  Serial.printf("[DHT] Status: %s\n", status);
  Serial.printf("[DHT] Temperature: %f\n", temperature);
  Serial.printf("[DHT] Humidity: %f\n", humidity);

  delay(60000);
}