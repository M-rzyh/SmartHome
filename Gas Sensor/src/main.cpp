#include <Arduino.h>
//int Buzzer = 6;        // used for Arduino
//int Gas_analog = A0;    // used for Arduino
//int Gas_digital = 7;   // used for Arduino

// int Buzzer = D2;        // used for ESP8266
int Gas_analog = A0;    // used for ESP8266
int Gas_digital = D1;   // used for ESP8266

// int Buzzer = 32;        // used for ESP32
// int Gas_analog = 4;    // used for ESP32
// int Gas_digital = 2;   // used for ESP32

void setup() {
  Serial.begin(115200);
  // pinMode(Buzzer, OUTPUT);      
  pinMode(Gas_digital, INPUT);
}

void loop() {
  int gassensorAnalog = analogRead(Gas_analog);
  int gassensorDigital = digitalRead(Gas_digital);

  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  
  if (gassensorAnalog > 1000) {
    Serial.println("Gas");
    // digitalWrite (Buzzer, HIGH) ; //send tone
    // delay(1000);
    // digitalWrite (Buzzer, LOW) ;  //no tone
  }
  else {
    Serial.println("No Gas");
  }
  delay(100);
}