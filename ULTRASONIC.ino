#define TRIG_PIN 5  // Trigger pin
#define ECHO_PIN 18 // Echo pin

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo pulse width
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Convert to distance (Speed of sound = 343m/s or 0.0343 cm/µs)
  float distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
