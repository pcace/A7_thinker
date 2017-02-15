int PWR_PIN = 2;

void setup() {

  // START A7 with 2 seconds 3,3V on PWR PIN
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(2000);
  digitalWrite(PWR_PIN, LOW);


}

void loop() {



}


