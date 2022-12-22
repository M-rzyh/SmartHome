#include <Arduino.h>

int data = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
}

void loop()
{
  delay(10);
  data = analogRead(A0);
  Serial.print("data: ");
  Serial.println(data);

  if (data < 400)
  {
    digitalWrite(D5, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(D5, LOW);
    Serial.println("haaaa por shod");
  }
}