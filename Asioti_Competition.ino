#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define FIREBASE_HOST "realtimecontrollerboat.firebaseio.com"
#define FIREBASE_AUTH "i5YBZLTwT3TAHIor3yKhdnPC0qP4bo8N8cjqyvY9"

#define RC_0 D0
#define RC_1 D1
#define RC_2 D2
#define RC_3 D3
#define RC_4 D4
#define RC_5 D5
#define RC_6 D6
#define RC_7 D7
#define RC_8 D8

FirebaseData firebaseData;
Servo PIN_0, PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8;
int RC0_VAL, RC1_VAL, RC2_VAL, RC3_VAL, RC4_VAL, RC5_VAL, RC6_VAL, RC7_VAL, RC8_VAL;
int speed_level = 180;

void setup()
{

  Serial.begin(115200);
  PIN_0.attach(RC_0,1000,2000);
  PIN_1.attach(RC_1,1000,2000);
  PIN_2.attach(RC_2,1000,2000);
  PIN_3.attach(RC_3,1000,2000);
  PIN_4.attach(RC_4,1000,2000);
  PIN_5.attach(RC_5,1000,2000);
  PIN_6.attach(RC_6,1000,2000);
  PIN_7.attach(RC_7,1000,2000);
  PIN_8.attach(RC_8,1000,2000);

  PIN_0.write(0);
  PIN_1.write(0);
  PIN_2.write(0);
  PIN_3.write(0);
  PIN_4.write(0);
  PIN_5.write(0);
  PIN_6.write(0);
  PIN_7.write(0);
  PIN_8.write(0);
    
  WiFiManager wifiManager;
  wifiManager.autoConnect("Device_1");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  delay(10);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!Firebase.beginStream(firebaseData, "JoyStick_1"))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
  

void loop()
{  
  stream();
  sendPWM();
}

//FUNCTION
void debugging()
{
  Serial.print("RC_0 : ");Serial.println(RC0_VAL);
  Serial.print("RC_1 : ");Serial.println(RC1_VAL);
  Serial.print("RC_2 : ");Serial.println(RC2_VAL);
  Serial.print("RC_3 : ");Serial.println(RC3_VAL);
  Serial.print("RC_4 : ");Serial.println(RC4_VAL);
  Serial.print("RC_5 : ");Serial.println(RC5_VAL);
  Serial.print("RC_6 : ");Serial.println(RC6_VAL);
  Serial.print("RC_7 : ");Serial.println(RC7_VAL);
  Serial.print("RC_8 : ");Serial.println(RC8_VAL);
}

void stream()
{
  if (!Firebase.readStream(firebaseData))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (firebaseData.streamAvailable())
  {
    Serial.println("------------------------------------");
    Serial.println("EVENT PATH: " + firebaseData.dataPath());
    Serial.println(firebaseData.stringData());
    Serial.println("------------------------------------");
    Serial.println();
    delay(50);
    
    if(firebaseData.dataPath().equals("/D0"))
    {
      RC0_VAL = firebaseData.stringData().toInt();
      speed_level = RC0_VAL;
    }
    if(firebaseData.dataPath().equals("/D1"))
    {
      RC1_VAL = firebaseData.stringData().toInt();
      RC1_VAL = map(RC1_VAL, 0, 150, 0, speed_level);
    }
    if(firebaseData.dataPath().equals("/D2"))
    {
      RC2_VAL = firebaseData.stringData().toInt();
      RC2_VAL = map(RC2_VAL, 0, 210, 0, 180);
    }
    if(firebaseData.dataPath().equals("/D3"))
    {
      RC3_VAL = firebaseData.stringData().toInt();
    }
    if(firebaseData.dataPath().equals("/D4"))
    {
      RC4_VAL = firebaseData.stringData().toInt();
    }
    if(firebaseData.dataPath().equals("/D5"))
    {
      RC5_VAL = firebaseData.stringData().toInt();
    }
    if(firebaseData.dataPath().equals("/D6"))
    {
      RC6_VAL = firebaseData.stringData().toInt();
    }
    if(firebaseData.dataPath().equals("/D7"))
    {
      RC7_VAL = firebaseData.stringData().toInt();
    }
    if(firebaseData.dataPath().equals("/D8"))
    {
      RC8_VAL = firebaseData.stringData().toInt();
    }
  }
}

void sendPWM()
{
  PIN_0.write(RC0_VAL);
  PIN_1.write(RC1_VAL);
  PIN_2.write(RC2_VAL);
  PIN_3.write(RC3_VAL);
  PIN_4.write(RC4_VAL);
  PIN_5.write(RC5_VAL);
  PIN_6.write(RC6_VAL);
  PIN_7.write(RC7_VAL);
  PIN_8.write(RC8_VAL);
}
