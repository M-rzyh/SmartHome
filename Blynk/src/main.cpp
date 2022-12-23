#include <Arduino.h>


#define BLYNK_TEMPLATE_ID "TMPLH_gJp0d4"
#define BLYNK_DEVICE_NAME "smart home"
#define BLYNK_AUTH_TOKEN "CBtiwIWJSZ7CYdM6PS9S89dlTjt8cCaw"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
int pool = 0;
int temp = 25;
int value2 = 0;
int value = 0;
int m = millis();
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "mohsen";
char pass[] = "m1111111";

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V0)//LED order
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V0 value is: ");
  Serial.println(pinValue);
  Blynk.virtualWrite(V1, pinValue); //test
  Blynk.virtualWrite(V8, pinValue);//test
}
BLYNK_WRITE(V4)//pool order
{
  pool = param.asInt();
  
  Serial.print("V4 value is: ");
  Serial.println(pool);

}
BLYNK_WRITE(V7)//temperature order
{
  temp = param.asInt();
  
  Serial.print("V7 value is: ");
  Serial.println(temp);

}
BLYNK_WRITE(V5)//watering
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

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();

}
void setup()
{
  // Debug console
  Serial.begin(115200);

  //gas state
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  
  // Blynk.syncAll();
  //pool and temperature test
  if(millis() - m > 500)
  {
    if(value < pool)
      Blynk.virtualWrite(V3, value++);
    else if(value > pool)
      Blynk.virtualWrite(V3, value--);
    if(value2 < temp)
      Blynk.virtualWrite(V6, value2++);

    else if(value2 > temp)
      Blynk.virtualWrite(V6, value2--);


    m+=500;
    Blynk.run();
    // Blynk.virtualWrite(V3, value);
    // Serial.println(value);
    // Serial.println(value2);
    // Blynk.virtualWrite(V6, value2);
  }
  //end pool and temperature test

}