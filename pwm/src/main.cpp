#include <Arduino.h>
const int LEDPIN = 16;
const int FREQ = 5000;
const int LEDCHANNEL = 0;
const int RES = 8;

void setup()
{
  ledcSetup(LEDCHANNEL, FREQ, RES);
  ledcAttachPin(LEDPIN, LEDCHANNEL);
}

void loop()
{
  for (int dutyCycle = 0; dutyCycle <= 225; dutyCycle++)
  {
    ledcWrite(LEDCHANNEL, dutyCycle);
    delay(15);
  }
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)
  {
    // changing the LED brightness with PWM
    ledcWrite(LEDCHANNEL, dutyCycle);
    delay(15);
  }
}