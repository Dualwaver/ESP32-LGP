#include <Arduino.h>
#include "../lib/utils/EspNow_util.h"

void setup() {
  Serial.begin(115200);
  /* Init */
  initEspNow(OnDataReceive,NULL);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}