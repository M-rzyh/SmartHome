#include <Arduino.h>
const int LEDPIN = 16;
const int FREQ = 5000;
const int LEDCHANNEL = 0;
const int RES = 8;

int sensorRead{0};
float voltage{0};
float tempreture{0};
float myTemp{10.0};
float error{0}, firstError{0};

float roomTemp()
{
  sensorRead = analogRead(34);
  voltage = sensorRead * (5000 / 1024.0);
  tempreture = (voltage - 500) / 10;
  Serial.print(" tempreture: ");
  Serial.println(tempreture);
  delay(100);
  return tempreture;
}
void setup()
{
  Serial.begin(9600);
  ledcSetup(LEDCHANNEL, FREQ, RES);
  ledcAttachPin(LEDPIN, LEDCHANNEL);
  firstError = roomTemp() - myTemp;
}

void loop()
{
  error = roomTemp() - myTemp;
  if (error > firstError)
  {
    firstError = roomTemp() - myTemp;
  }
  if (error > 0.0)
  {
    ledcWrite(LEDCHANNEL, error * 0.95 * (255.0 / 10.0));
    delay(15);
  }
  else
  {
    ledcWrite(LEDCHANNEL, 0);
  }
}