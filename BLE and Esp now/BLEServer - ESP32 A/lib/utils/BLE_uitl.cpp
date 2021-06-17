#include "util.h"
#include "BLE_util.h"
#include <Arduino.h>

void notifyBLE(BLECharacteristic* pCharacteristic, message msgR, message msgY, char* msgBLE){
    if(msgR.ledState == msgY.ledState)
      strcpy(msgBLE,msgR.ledState?"Red and Yellow ON":"Red and Yellow ON");
    else if(msgR.ledState)
      strcpy(msgBLE,"Red ON Yellow OFF");
    else
      strcpy(msgBLE,"Red OFF Yellow ON");
    
    Serial.println(msgBLE);
    pCharacteristic->setValue((char*)msgBLE);
    pCharacteristic->notify();
}