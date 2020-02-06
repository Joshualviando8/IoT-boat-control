#include <SoftwareSerial.h>

#define trigger1 9
#define trigger2 7
#define echo1 8
#define echo2 6

SoftwareSerial proSerial(2,3);

void setup() {
  Serial.begin (9600);
  proSerial.begin(19200);
}

void loop() {
  if(Serial.available()>0)
  {
    if(Serial.read() == 'a')
    {
      Serial.println("Mengirim data . . .");
      proSerial.print("hallo dari promicro");
    }
  }
  /*if (ArduinoUno.available() > 0) {
    Serial.println("Data dari NodeMCU:");
    float data_recieve = ArduinoUno.parseFloat();
    Serial.println(data_recieve);
    if(data_recieve == NULL)
    {
      Serial.println("no data");
    }
    delay(10);
  }*/
}
