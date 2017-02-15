int PWR_PIN = 2;
String content = "";
String character;
void setup() {

  // START A7 with 2 seconds 3,3V on PWR PIN

  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(1000);
  digitalWrite(PWR_PIN, LOW);

  // SETUP SERIAL INTERFACE

  Serial.begin(9600);
  Serial1.begin(112500);
  Serial.println("INIT");
}



void loop() {
 
}
