#include "DHT.h"

#define DHTPIN 4  // GPIO pin connected to DHT22 data pin
#define DHTTYPE DHT22  // Using DHT22 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200 );  // Higher baud rate for ESP32
  Serial.println(F("DHT22 Sensor Test on ESP32"));

  dht.begin();
}

void loop() {
  delay(2000);  // Delay between readings (DHT22 needs 2s)

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
