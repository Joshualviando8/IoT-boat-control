#include <SoftwareSerial.h>
SoftwareSerial NodeMCU(D6,D5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  NodeMCU.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (NodeMCU.available() > 0) {
      Serial.println("Data dari Arduino:");
      //float data_recieve = NodeMCU.parseFloat();
      String data_recieve = NodeMCU.readString();
      Serial.println(data_recieve);
      if(data_recieve == NULL)
    {
      Serial.println("ERROR");
    }
  }
}
