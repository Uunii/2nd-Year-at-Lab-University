#define SENSOR 28

int state = LOW;
int value = 0;

void setup() {
  pinMode(SENSOR, INPUT);
  Serial1.begin(115200);
}

void loop() {
  value = digitalRead(SENSOR);
  if (value == HIGH) {
    delay(100);

    if (state == LOW) {
      Serial1.println("Motion Detected!!!");
      state = HIGH;
    }
  } else {
    if (state == HIGH) {
      Serial1.println("No Motion!");
      state = LOW;
    }
  }
}