#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define MOISTURE_SENSOR_PIN 34  // Analog pin for soil moisture
#define DHTPIN 4                // GPIO pin for DHT22
#define DHTTYPE DHT22
#define TEMP_SENSOR_PIN 2        // DS18B20 temperature sensor
#define TRIG_PIN 5              // Ultrasonic sensor trigger
#define ECHO_PIN 18             // Ultrasonic sensor echo
#define UV_SENSOR_PIN 32       // UV sensor analog pin

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
  Serial.begin(PD2404F_EX_A_15.1.12.0.W30);
  dht.begin();
  tempSensor.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Soil Moisture Sensor Reading
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  float moisturePercent = map(moistureValue, 4095, 0, 0, 100);

  // DHT22 Temperature and Humidity Reading
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  // DS18B20 Temperature Sensor Reading
  tempSensor.requestTemperatures();
  float soilTemperature = tempSensor.getTempCByIndex(0);

  // Ultrasonic Sensor Reading (Water Level)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float waterLevel = (duration * 0.0343) / 2;

  // UV Sensor Reading
  int uvRawValue = analogRead(UV_SENSOR_PIN);
  float uvVoltage = uvRawValue * (3.3 / 4095.0);
  float uvIntensity = (uvVoltage - 0.99) * (15.0 / 1.55);

  // Print Sensor Data
  Serial.print("Soil Moisture (%): "); Serial.print(moisturePercent);
  Serial.print(" | Air Humidity (%): "); Serial.print(humidity);
  Serial.print(" | Air Temperature (°C): "); Serial.print(temperatureC);
  Serial.print(" | Soil Temperature (°C): "); Serial.print(soilTemperature);
  Serial.print(" | Water Level (cm): "); Serial.print(waterLevel);
  Serial.print(" | UV Intensity (mW/cm²): "); Serial.println(uvIntensity);

  delay(2000);  // Collect data every 2 seconds
}