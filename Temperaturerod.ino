#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4  // GPIO pin connected to DS18B20 data pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();  // Request temperature reading
  float temperatureC = sensors.getTempCByIndex(0); // Get temperature in Celsius

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  delay(1000); // Read every second
}
