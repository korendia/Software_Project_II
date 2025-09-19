#define LED_PIN 7

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, LOW);   // ON
    delay(100);
    digitalWrite(LED_PIN, HIGH);  // OFF
    delay(100);
  }

  digitalWrite(LED_PIN, HIGH);    // OFF
  
  while (1) {}
}
