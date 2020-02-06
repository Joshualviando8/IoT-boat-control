//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

#define FIREBASE_HOST "realtimecontrollerboat.firebaseio.com" //Without http:// or https://
#define FIREBASE_AUTH "i5YBZLTwT3TAHIor3yKhdnPC0qP4bo8N8cjqyvY9"

#define throttle_1 D5
#define throttle_2 D6
#define nozzle_1 D7
#define nozzle_2 D8

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
Servo motor_1, motor_2, servo_1, servo_2;

//
int pwm1_val = 0;
int pwm2_val = 0;
int pwm3_val = 90;
int pwm4_val = 180;
int on_manual_turn = 0;

int tempo;
int duration, distance;

String pwm_thruster = "/Throttle";
String pwm_nozzle = "/Nozzel";
String pwm_SpeedLevel = "/SpeedLevel";
String pwm_kirikanan = "/kirikanan";

int speed_limiter = 0;

void setup()
{

  Serial.begin(115200);
  
  motor_1.attach(throttle_1,1000,2000);
  motor_2.attach(throttle_2,1000,2000);
  servo_1.attach(nozzle_1,1000,2000);
  servo_2.attach(nozzle_2,1000,2000);

  motor_1.write(0);
  motor_2.write(0);
  servo_1.write(90);
  servo_2.write(90);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("Kapal_Tenggelam_1");
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
  if(on_manual_turn == 1)
  {
    kiri_kanan_manual();
  }
  sendPWM();
}


/////////function///////
void debugging()
{
  Serial.print("throttle : ");Serial.println(pwm1_val);
  Serial.print("Nozzle : ");Serial.println(pwm3_val);
  Serial.print("speed level : ");Serial.println(pwm4_val);
}

void sendPWM()
{
  motor_1.write(pwm1_val);
  motor_2.write(pwm2_val);
  servo_1.write(pwm3_val);
  servo_2.write(pwm3_val);
}

void kiri_kanan_manual()
{
  if(pwm3_val == 90)
  {
    pwm1_val = tempo;
    pwm2_val = pwm1_val;
  }
  else if(pwm3_val > 130)
  {
    pwm1_val = 0;
  }
  else if(pwm3_val < 50)
  {
    pwm2_val = 0;
  }
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
    if(firebaseData.dataType() == "string")
    {
      Serial.println(firebaseData.stringData());
    }
    else if(firebaseData.dataType() == "json")
    {
      Serial.println(firebaseData.jsonData());
    }
    
    Serial.println("------------------------------------");
    Serial.println();
    delay(50);
    
    if(firebaseData.dataPath().equals(pwm_nozzle))
    {
      pwm3_val = map((firebaseData.stringData().toInt()), 0, 210, 0, 180);
      Serial.println(pwm3_val);
    }

    if(firebaseData.dataPath().equals(pwm_kirikanan))
    {
      pwm1_val = map((firebaseData.stringData().toInt()), 0, 150, 0, speed_limiter);
      pwm2_val = pwm1_val;
      tempo = pwm1_val;
    }


    if(firebaseData.dataPath().equals(pwm_SpeedLevel))
    {
      pwm4_val = map((firebaseData.stringData().toInt()), 1, 4, 0, 180);
      
      if(pwm4_val ==0) //50 % speed
      {
        speed_limiter = 90;
      }
      else if(pwm4_val ==60) //60% speed
      {
        speed_limiter = 120;
      }
      else if(pwm4_val==120) // 85% speed
      {
        speed_limiter = 150;
      }
      else if(pwm4_val==180) //100% speed
      {
        speed_limiter = 180;
      }
      
      
    }

    if(firebaseData.dataPath().equals(pwm_thruster))
    {
      pwm1_val = map((firebaseData.stringData().toInt()), 0, 150, 0, speed_limiter);
      pwm2_val = pwm1_val;
      tempo = pwm1_val;
    }

    if(firebaseData.dataPath().equals(pwm_kirikanan))
    {
      pwm1_val = firebaseData.stringData().toInt();
    }
    
  }
}
