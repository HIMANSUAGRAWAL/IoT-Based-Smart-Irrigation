#define SENSOR_PIN 34  // Analog pin connected to sensor

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  
  // Convert to percentage (0 = Dry, 4095 = Wet for ESP32)
  float moisturePercent = map(sensorValue, 4095, 0, 0, 100);
  
  Serial.print("Raw Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  delay(1000);
}
