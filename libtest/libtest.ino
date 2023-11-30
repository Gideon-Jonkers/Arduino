#include "geyserwise.h"

GeyserWiseComm monit(D6,D5);
GeyserWiseComm logger(D2,D1);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  monit.init();
  logger.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t pack[4] = {00,70,00,24};
  monit.transmitHandler(logger.getLastPack());  // towards monit
  logger.transmitHandler(monit.getLastPack()); // towards logger
  //delay(1000);
  Serial.print("From monitor->");
  Serial.println(monit.GetLastMsg(),HEX);
  Serial.print("From Element->");
  Serial.println(logger.GetLastMsg(),HEX);
  
}
