#include <TinyGPS.h>

int PWR_PIN = 2;
String content = "";
String character;


void setup() {

  pinMode(PWR_PIN, OUTPUT);
  Serial1.begin(115200);
  Serial.begin(9600);

  Serial.println("Starting...");
  power_on();

  delay(3000);

  // sets the PIN code
  //sprintf(aux_string, "AT+CPIN=%s", pin);
  //sendATcommand(aux_string, "OK", 2000);

  delay(3000);

  Serial.println("Connecting to the network...");

  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
           sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  Serial.print("Setting SMS mode...");
  sendATcommand("AT+CMGF=1", "OK", 1000);
  Serial.print("Setting Memory Locations...");
  sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000); 
  
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

/*SEND SMS 
 * 
 * 
 *     Serial.println("Sending SMS");
    
    sprintf(aux_string,"AT+CMGS=\"%s\"", phone_number);
    answer = sendATcommand(aux_string, ">", 2000);    // send the SMS number
    if (answer == 1)
    {
        Serial.println(sms_text);
        Serial.write(0x1A);
        answer = sendATcommand("", "OK", 20000);
        if (answer == 1)
        {
            Serial.print("Sent ");    
        }
        else
        {
            Serial.print("error ");
        }
    }
    else
    {
        Serial.print("error ");
        Serial.println(answer, DEC);
    }

 * 
 */



  /*
   * READ SMS FROM MEMORY
    answer = sendATcommand("AT+CMGR=1", "+CMGR:", 2000);    // reads the first SMS
    if (answer == 1)
    {
        answer = 0;
        while(Serial.available() == 0);
        // this loop reads the data of the SMS
        do{
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            if(Serial.available() > 0){    
                SMS[x] = Serial.read();
                x++;
                // check if the desired answer (OK) is in the response of the module
                if (strstr(SMS, "OK") != NULL)    
                {
                    answer = 1;
                }
            }
        }while(answer == 0);    // Waits for the asnwer with time out
        
        SMS[x] = '\0';
        
        Serial.print(SMS);    
        
    }
    else
    {
        Serial.print("error ");
        Serial.println(answer, DEC);
    }

   */




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
    if (Serial.available() != 0) {
      response[x] = Serial.read();
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
