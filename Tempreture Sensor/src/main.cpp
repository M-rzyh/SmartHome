#include <Arduino.h>
int sensorRead{0};
float voltage{0};
float tempreture{0};
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensorRead = analogRead(34);
  Serial.print("SensorData: ");
  Serial.print(sensorRead);
  voltage = sensorRead * (5000/1024.0);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  tempreture = (voltage - 500) / 10;
  Serial.print(" tempreture: ");
  Serial.println(tempreture);
  delay(500);
}