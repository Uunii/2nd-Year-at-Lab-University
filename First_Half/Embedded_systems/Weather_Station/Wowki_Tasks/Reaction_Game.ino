#include <Arduino.h>

#define LED 15
#define BUTTON 14

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial1.begin(115200);
}

void loop() {
  Serial1.println("\nWait for RED led to turn off!");
  digitalWrite(LED, HIGH);
  delay(random(1000, 5000));

  digitalWrite(LED, LOW);
  uint64_t Start = time_us_64();

  while (digitalRead(BUTTON) == HIGH) {
  }
  uint64_t End = time_us_64();
  uint64_t Time = (End - Start) / 1000;

  Serial1.print("Reaction time: ");
  Serial1.print(Time);
  Serial1.println(" ms");
  delay(2000);
}
