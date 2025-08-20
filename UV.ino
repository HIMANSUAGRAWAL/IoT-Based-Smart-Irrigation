#define UV_SENSOR_PIN 34  // Analog pin connected to OUT

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(UV_SENSOR_PIN);
  
  // Convert raw value (0-4095) to voltage (ESP32 ADC: 3.3V reference)
  float voltage = sensorValue * (3.3 / 4095.0);
  
  // Convert voltage to UV intensity (mW/cm²)
  float uvIntensity = (voltage - 0.99) * (15.0 / 1.55);  // Adjust based on calibration
  
  Serial.print("Raw Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.print("V | UV Intensity: ");
  Serial.print(uvIntensity);
  Serial.println(" mW/cm²");
  
  delay(1000);
}
