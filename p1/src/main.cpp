#include <Arduino.h>

int data = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(35, INPUT);
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
}

void loop()
{
  delay(10);
  data = analogRead(35);
  Serial.print("data: ");
  Serial.print(data);

  if (data < 400)
  {
    digitalWrite(21, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(21, LOW);
    Serial.print("haaaa por shod");
  }
}
// ledcWrite