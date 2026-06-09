#define RED     22
#define YELLOW  21
#define GREEN   20
#define BUTTON  15
#define BUZZER  14

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
  delay(1000);
  if (digitalRead(BUTTON) == LOW) {
    Buzzer();
    return;
  }

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
  delay(1000);
  if (digitalRead(BUTTON) == LOW) {
    Buzzer();
    return;
  }  

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);
  delay(1000);
  if (digitalRead(BUTTON) == LOW) {
    Buzzer();
    return;
  }  

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
  delay(1000);
  if (digitalRead(BUTTON) == LOW) {
    Buzzer();
    return;
  }  
}

void Buzzer() {
  tone(BUZZER, 100);
  delay(200);
  noTone(BUZZER);
}