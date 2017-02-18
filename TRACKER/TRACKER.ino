#include <Arduino.h>
#include <NMEAGPS.h>
#include <TinyGPS.h>

int PWR_PIN = 2;
String content = "";
String character;
TinyGPS gps;

float lon, lat, sat, prec, alt, spd, crs;
long unsigned age, Date, Time;



int8_t answer;
int x;
char aux_string[30];
char SMS[200];



void setup() {

  pinMode(PWR_PIN, OUTPUT);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial.begin(9600);

  Serial.println("Starting...");
  power_on();

  delay(3000);

  delay(3000);

  Serial.println("Connecting to the network...");

  while ( (sendATcommand("AT+CREG?", "+CREG: 1,1", 500) || sendATcommand("AT+CREG?", "+CREG: 1,5", 500) ||
           sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );



  Serial.println("Connecting to the network... SUCCESS");
  Serial.println("Setting SMS-TEXT mode...");

  sendATcommand("AT+CMGF=1", "OK", 1000);
  Serial.println("Setting SMS-TEXT mode...SUCCESS");
  delay(500);
  Serial.println("Setting Memory Locations...");
  delay(500);
  sendATcommand("AT+CPMS=\"SM\",\"ME\",\"SM\"", "OK", 1000);
  Serial.println("Setting Memory Locations...SUCCESS (SM/ME/SM)");

  delay(500);
  Serial.println("Setting GPS mode...");
  sendATcommand("AT+GPS=1", "OK", 1000);
  Serial.println("Setting GPS mode...SUCCESS");

  delay(500);
  Serial.println("Setting AGPS mode...");
  sendATcommand("AT+AGPS=1", "OK", 1000);
  Serial.println("Setting AGPS mode...SUCCESS");
}


void loop() {
   while (true) {
     if (Serial1.available()) {
       Serial.write(Serial1.read());
     }
     if (Serial.available()) {
       Serial1.write(Serial.read());
     }
     delay(0);

    }
  //get_GPS(&lat, &lon, &sat, &prec, &age, &alt, &spd, &crs, &Date, &Time);
 
}



void get_GPS(float *lat, float *lon, float *sat, float *prec, long unsigned *age,  float *alt, float *spd, float *crs, long unsigned *Date, long unsigned *Time) {

  bool newData = false;
  long unsigned date, lst_time, fix_age;
  //*lat = 1;
  for (unsigned long start = millis(); millis() - start < 3000;)
  {
    while (Serial2.available())
    {
      char c = Serial2.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);



    *lat = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
    *lon = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
    *sat = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites();
    *prec = gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop();
    *alt = gps.f_altitude();
    *spd = gps.f_speed_kmph();
    *crs = gps.f_course();

    gps.get_datetime(&date, &lst_time, &fix_age);
    *Date = date;
    *Time = lst_time;
  }
}


void power_on() {

  uint8_t answer = 0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(PWR_PIN, HIGH);
    delay(3000);
    digitalWrite(PWR_PIN, LOW);

    // waits for an answer from the module
    while (answer == 0) {   // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);
    }
  }

}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialice the string

  delay(100);

  while ( Serial1.available() > 0) Serial1.read();   // Clean the input buffer

  Serial1.println(ATcommand);    // Send the AT command


  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (Serial1.available() != 0) {
      response[x] = Serial1.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  } while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}
