#include <Arduino.h>
#include <LM35.h>
#include <WiFi.h>
// #include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPLH_gJp0d4"
#define BLYNK_DEVICE_NAME "smart home"
#define BLYNK_AUTH_TOKEN "CBtiwIWJSZ7CYdM6PS9S89dlTjt8cCaw"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// #define Servo 15
#define LM 35
#define WaterSensor 32
#define Fan 33
#define Pomp_pool 13
#define Pomp_Watering 4
#define SensorPower 14
// #define Tempreture 4
#define Gas 34

// define RFID GPIOs
#define RFID_RST 0
#define RFID_MISO 19
#define RFID_MOSI 23
#define RFID_SDK 18
#define RFID_SDA 5

// define LED GPIOs
#define LED1 21
#define LED2 22
#define LED3 25
#define LED4 26
#define LED5 27
#define LED6 12

// define WiFi SSID and PSWD
#define SSID "mohsen"
#define PSWD "m1111111"

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

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
  // if (WaterSensorData < 400)
  // {
  //   digitalWrite(Pomp_pool, HIGH);
  //   Serial.println("1");
  // }
  // else
  // {
  //   digitalWrite(Pomp_pool, LOW);
  //   Serial.print("haaaa por shod");
  // }
  digitalWrite(Pomp_pool, HIGH);
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

  ADC_Tempreture = analogRead(LM);
  Serial.print("SensorData: ");
  Serial.print(ADC_Tempreture);
  TempretureVoltage = ADC_Tempreture * (5000 / 1024.0);
  Serial.print(" Voltage: ");
  Serial.print(TempretureVoltage);
  data = (TempretureVoltage - 500.0) / 10;
  Serial.print(" tempreture: ");
  Serial.println(data);
  delay(500);
}

void LEDSetup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  // digitalWrite(LED1, LOW);
  // digitalWrite(LED2, HIGH);
  // digitalWrite(LED3, HIGH);
  // digitalWrite(LED4, HIGH);
  // digitalWrite(LED5, HIGH);
  // digitalWrite(LED6, HIGH);
}

void BlynkSetup()
{
  char ssid[] = SSID;
  char pass[] = PSWD;
  Blynk.begin(auth, ssid, pass);
}

// LED1 Control Blynk Function
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();

  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();

  Serial.print("V0(LED1) value is: ");
  Serial.println(pinValue);

  // Write on the Server
  Blynk.virtualWrite(V1, pinValue);
  Blynk.virtualWrite(V8, pinValue);
}

// Adjusting the Pool Water Level
BLYNK_WRITE(V4) // pool order
{
  int adj_pool_water_level = param.asInt();

  Serial.print("V4(Pool) value is: ");
  Serial.println(adj_pool_water_level);
}

// Adjusting the Room Tempreture
BLYNK_WRITE(V7)
{
  int adj_room_tempreture = param.asInt();

  Serial.print("V7 value is: ");
  Serial.println(adj_room_tempreture);
}

// Waterig Timing
BLYNK_WRITE(V5)
{
  TimeInputParam t(param);

  // Process start time

  if (t.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }
  else if (t.isStartSunrise())
  {
    Serial.println("Start at sunrise");
  }
  else if (t.isStartSunset())
  {
    Serial.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (t.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
  }
  else if (t.isStopSunrise())
  {
    Serial.println("Stop at sunrise");
  }
  else if (t.isStopSunset())
  {
    Serial.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone
  // Timezone is already added to start/stop time

  Serial.println(String("Time zone: ") + t.getTZ());

  // Get timezone offset (in seconds)
  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++)
  {
    if (t.isWeekdaySelected(i))
    {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  GasSensorSetup();
  WaterSensorSetup();
  TempretureSensorSetup();
  // pinMode(21, LOW);

  // Blynk Setup and WiFi connection
  BlynkSetup();

  // LED PinMode Setup
  LEDSetup();
}

void loop()
{

  // DetectGas();//test done
  PoolPomp();
  // TempretureSensor();
  // Serial.print("Temp - ");  // Send a text to serial
  // Serial.print(temp.cel()); // It gets the temperature in celcius and send to serial
  // Serial.print(" C  - ");
  // Serial.print(temp.fah()); // It gets the temperature in fahrenheit and send to serial
  // Serial.print(" F  - ");
  // Serial.print(temp.kel()); // It gets the temperature in kelvin and send to serial
  // Serial.println(" K");     // Send a text to serial and give a new line
  // Serial.println(" ");

  // Blynk loop function (do not remove or coment this)
  Blynk.run();
}