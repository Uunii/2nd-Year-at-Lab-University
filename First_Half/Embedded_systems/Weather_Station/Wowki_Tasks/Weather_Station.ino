#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* thingSpeakAPIKey = "4T5N9CVQ6UCZ38ZW";
const char* thingSpeakServer = "http://api.thingspeak.com/update";

void setup() {
  Serial1.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial1.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial1.println("Connecting to WiFi...");
  }
  Serial1.println("Connected to WiFi.");
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial1.print("Temp: ");
  Serial1.print(t);
  Serial1.print("°C ");
  Serial1.print("Humidity: ");
  Serial1.print(h);
  Serial1.println("%");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(thingSpeakServer) +
                 "?api_key=" + thingSpeakAPIKey +
                 "&field1=" + String(t) +
                 "&field2=" + String(h);
  http.begin(url);
  int httpCode = http.GET();
  http.end();
  }
  delay(30000);
}
