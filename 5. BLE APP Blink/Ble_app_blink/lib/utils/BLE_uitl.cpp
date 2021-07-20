#include "util.h"
#include "BLE_util.h"
#include <Arduino.h>

void messageToUint8 (message_t *message, uint8_t *send, uint8_t hand){
  send[0] = hand;
  send[1] = (uint8_t) message->local;

  for (size_t i = 2; i < BLE_SEND_SIZE; i += 4){
    if (i < 6)
      memcpy(&send[i],&message->x,sizeof(float));
    else if(i < 10)
      memcpy(&send[i],&message->y,sizeof(float));
    else
      memcpy(&send[i],&message->z,sizeof(float));
  }
}

// void notifyBLE(BLECharacteristic* pCharacteristic, message msgR, message msgY, char* msgBLE){
//     if(msgR.ledState == msgY.ledState)
//       strcpy(msgBLE,msgR.ledState?"Red and Yellow ON":"Red and Yellow ON");
//     else if(msgR.ledState)
//       strcpy(msgBLE,"Red ON Yellow OFF");
//     else
//       strcpy(msgBLE,"Red OFF Yellow ON");
    
//     Serial.println(msgBLE);
//     pCharacteristic->setValue((char*)msgBLE);
//     pCharacteristic->notify();
// }

void notifyBLE(BLECharacteristic* pCharacteristic, message_t msgR, message_t msgY, char* msgBLE){
  
}