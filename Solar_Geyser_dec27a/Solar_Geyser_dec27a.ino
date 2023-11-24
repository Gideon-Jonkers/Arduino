#include "arduino_secrets.h"
/*
  Sketch generated by the Arduino IoT Cloud Thing "Solar Geyser"
  https://create.arduino.cc/cloud/things/469f79e3-f760-4975-8a77-7bd420c86be4

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudTemperature temperature;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

void setup() {
  pinMode(2,OUTPUT);
  pinMode(13,OUTPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  digitalWrite(13,HIGH);
}

void loop() {
  ArduinoCloud.update();
  // Your code here
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(55000);
  temperature = analogRead(A0);

}