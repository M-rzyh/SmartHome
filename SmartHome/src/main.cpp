#include <Arduino.h>
#include <LM35.h>
#include <WiFi.h>
// #include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>     //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h> //https://github.com/miguelbalboa/rfid
#include <ESP32Servo.h>

#define SS_PIN 5
#define RST_PIN 0
const int ipaddress[4] = {103, 97, 67, 25};
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
Servo myservo;

#define BLYNK_TEMPLATE_ID "TMPLH_gJp0d4"
#define BLYNK_DEVICE_NAME "smart home"
#define BLYNK_AUTH_TOKEN "CBtiwIWJSZ7CYdM6PS9S89dlTjt8cCaw"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// #define Servo 15
#define LM 35
#define WaterSensor 32
#define FAN 33
#define Pomp_pool 13
#define Pomp_Watering 4
#define SensorPower 14
// #define Tempreture 4
#define Gas 34
#define SERVO 15

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

// Tempreture
//  int TempretureSensorData = 0;
float TempretureVoltage = 0;
float data = 0;
int ADC_Tempreture = 0;
LM35 temp(LM);

// Fan
const int FREQ = 5000;
const int FANCHANNEL = 0;
const int RES = 8;
int sensorRead{0};
float voltage{0};
float tempreture{0};
float myTemp{10.0}; // initial one
float error{0}, firstError{0};

void servoSetup()
{
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo
  myservo.attach(SERVO, 500, 2400);
}
void RFIDSetup()
{
  // Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //  myservo.attach(6);
  //  myservo.write(0);
  // init rfid D8,D5,D6,D7
  SPI.begin();
  rfid.PCD_Init();

  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
}
void readRFID(void)
{ /* function readRFID */
  ////Read RFID card

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  if (rfid.uid.uidByte[0] == 113 && rfid.uid.uidByte[1] == 235 && rfid.uid.uidByte[2] == 181 && rfid.uid.uidByte[3] == 36)
  {
    myservo.write(90);
    delay(1000);
    myservo.write(0);
  }

  Serial.print(F("RFID In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void sensorPower()
{
  pinMode(SensorPower, OUTPUT);
  digitalWrite(SensorPower, HIGH);
}

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

void WateringSetup()
{
  pinMode(Pomp_Watering, OUTPUT);
  digitalWrite(Pomp_Watering, LOW);
}

void FanSetup()
{
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, LOW);
  ledcSetup(FANCHANNEL, FREQ, RES);
  ledcAttachPin(FAN, FANCHANNEL);
  firstError = TempretureSensor() - myTemp;
}
void Fan()
{
  error = TempretureSensor() - myTemp;
  if (error > firstError)
  {
    firstError = TempretureSensor() - myTemp;
  }
  if (error > 0.0)
  {
    ledcWrite(FANCHANNEL, error * 0.95 * (255.0 / 10.0));
    delay(15);
  }
  else
  {
    ledcWrite(FANCHANNEL, 0);
  }
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
  delay(1000);
  digitalWrite(Pomp_pool, LOW);
  delay(1000);

  // ledcSetup(0, FREQ, RES);
  // ledcAttachPin(Pomp_pool, 0);
}

void TempretureSensorSetup()
{
  pinMode(LM, INPUT);
}
float TempretureSensor()
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
  return data;
}

void LEDSetup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
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
  sensorPower();

  // sensorSetup
  GasSensorSetup();
  WaterSensorSetup();
  TempretureSensorSetup();
  FanSetup();
  WateringSetup();

  // Blynk Setup and WiFi connection
  // BlynkSetup();

  // LED PinMode Setup
  LEDSetup();

  // test
  // pinMode(RFID_MOSI, OUTPUT);
  // digitalWrite(RFID_MOSI, HIGH);
}

void loop()
{
  // DetectGas();//test done
  PoolPomp();
  // TempretureSensor();
  Fan();
  Serial.println("  ");
  // Blynk loop function (do not remove or coment this)
  // Blynk.run();
}