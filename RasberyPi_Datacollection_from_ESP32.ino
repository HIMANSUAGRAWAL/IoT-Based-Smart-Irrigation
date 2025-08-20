#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <ArduinoJson.h>

#define MOISTURE_SENSOR_PIN 34  
#define DHTPIN 4                
#define DHTTYPE DHT22
#define TEMP_SENSOR_PIN 2        
#define TRIG_PIN 5              
#define ECHO_PIN 18             
#define UV_SENSOR_PIN 32       

const char* ssid = "EMBB CUTM";    
const char* password = "embb@#cutm";    
const char* mqtt_broker = "10.1.3.84";  
const char* mqtt_topic = "sensor/data";  

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_broker, 1883);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("Failed. Retrying in 5s...");
      delay(500);
    }
  }

  dht.begin();
  tempSensor.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    client.connect("ESP32_Client");
  }
  client.loop();

  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  float moisturePercent = map(moistureValue, 4095, 0, 0, 100);

  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  tempSensor.requestTemperatures();
  float soilTemperature = tempSensor.getTempCByIndex(0);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float waterLevel = (duration * 0.0343) / 2;

  int uvRawValue = analogRead(UV_SENSOR_PIN);
  float uvVoltage = uvRawValue * (3.3 / 4095.0);
  float uvIntensity = (uvVoltage - 0.99) * (15.0 / 1.55);

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["soil_moisture"] = moisturePercent;
  jsonDoc["humidity"] = humidity;
  jsonDoc["air_temperature"] = temperatureC;
  jsonDoc["soil_temperature"] = soilTemperature;
  jsonDoc["water_level"] = waterLevel;
  jsonDoc["uv_intensity"] = uvIntensity;

  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  client.publish(mqtt_topic, jsonBuffer);
  Serial.println("Data sent via MQTT!");

  delay(2000);
}
