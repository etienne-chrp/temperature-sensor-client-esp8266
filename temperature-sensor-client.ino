#include "conf/wifi-config.h"
#include "conf/httpclient-config.h"

#include <DHTesp.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

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

  // Ensure that WiFi is connected
  if (WiFi.status() != WL_CONNECTED)
    return;

  // HTTPS Client
  WiFiClientSecure client;
  // Disable certificate verification
  client.setInsecure();

  HTTPClient https;

  String content = "{\"temperature\": ";
  content = content +
    temperature + ", " +
    "\"humidity\": " + humidity +
    "}";

  Serial.printf("[HTTP] Send POST: %s\n", url);

  https.begin(client, url);
  https.addHeader("Content-Type", "application/json");
  int httpCode = https.POST(content);

  // httpCode will be negative on error
  if (httpCode > 0) {
    Serial.printf("[HTTP] Reponse code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const char* response = https.getString().c_str();
      Serial.printf("[HTTP] Response content: %s\n", response);
    }
  }
  else {
    Serial.printf("[HTTP] Request failed: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();

  delay(60000);
}