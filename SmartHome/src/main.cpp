#include <Arduino.h>
#include <LM35.h>

// #define Servo 15
#define LM 35
#define WaterSensor 32
#define Fan 33
#define Pomp_pool 13
#define Pomp_Watering 4
#define SensorPower 14
// #define Tempreture 4
#define Gas 34

#define RFID_RST 0
#define RFID_MISO 19
#define RFID_MOSI 23
#define RFID_SDK 18
#define RFID_SDA 5

#define LED1 21
#define LED2 22
#define LED3 25
#define LED4 26
#define LED5 27
#define LED6 12

int WaterSensorData = 0;
// int TempretureSensorData = 0;
float TempretureVoltage = 0;
float data = 0;
int ADC_Tempreture = 0;
LM35 temp(LM);

void GasSensorSetup()
{
  pinMode(Gas, INPUT);
}
void DetectGas()
{
  int gassensorAnalog = analogRead(Gas);
  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");

  if (gassensorAnalog > 1000)
  {
    Serial.println("Gas");
    // digitalWrite (Buzzer, HIGH) ; //send tone
    // delay(1000);
    // digitalWrite (Buzzer, LOW) ;  //no tone
  }
  else
  {
    Serial.println("No Gas");
  }
  delay(100);
}

void WaterSensorSetup()
{
  pinMode(WaterSensor, INPUT);
  pinMode(Pomp_pool, OUTPUT);
  digitalWrite(Pomp_pool, LOW);
}
void PoolPomp()
{
  WaterSensorData = analogRead(WaterSensor);
  Serial.print("WaterHeight: ");
  Serial.print(WaterSensorData);
  if (WaterSensorData < 400)
  {
    digitalWrite(Pomp_pool, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(Pomp_pool, LOW);
    Serial.print("haaaa por shod");
  }
}
void TempretureSensorSetup()
{
  pinMode(LM, INPUT);
}
void TempretureSensor()
{
  // TempretureSensorData = analogRead(LM35);
  // Serial.print("SensorData: ");
  // Serial.print(TempretureSensorData);
  // TempretureVoltage = TempretureSensorData * (5000 / 1024.0);
  // Serial.print(" Voltage: ");
  // Serial.print(TempretureVoltage);
  // data = (TempretureVoltage - 500) / 10;
  // Serial.print(" tempreture: ");
  // Serial.println(data);
  // delay(500);

  // ADC_Tempreture = analogRead(LM35);
  // Serial.print("SensorData: ");
  // Serial.print(ADC_Tempreture);
  // TempretureVoltage = ADC_Tempreture * (5000 / 1024.0);
  // Serial.print(" Voltage: ");
  // Serial.print(TempretureVoltage);
  // data = (TempretureVoltage - 500.0) / 10;
  // Serial.print(" tempreture: ");
  // Serial.println(data);
  // delay(500);
}

void setup()
{
  Serial.begin(115200);
  GasSensorSetup();
  WaterSensorSetup();
  TempretureSensorSetup();
  pinMode(21, LOW);
}

void loop()
{
  // DetectGas();//test done
  // PoolPomp();
  // TempretureSensor();
  Serial.print("Temp - ");  // Send a text to serial
  Serial.print(temp.cel()); // It gets the temperature in celcius and send to serial
  Serial.print(" C  - ");
  Serial.print(temp.fah()); // It gets the temperature in fahrenheit and send to serial
  Serial.print(" F  - ");
  Serial.print(temp.kel()); // It gets the temperature in kelvin and send to serial
  Serial.println(" K");     // Send a text to serial and give a new line
  Serial.println(" ");
}