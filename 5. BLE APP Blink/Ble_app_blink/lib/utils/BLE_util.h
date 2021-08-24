#ifndef BLE_UTIL_H_
#define BLE_UTIL_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "util.h"

//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SERVICE_UUID "544b335f-aa69-4be8-aef4-ca32a9867832"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID "e8695a15-7f64-4432-89af-2e42054399bb"

#define BLE_SEND_SIZE 14

void notifyBLE(BLECharacteristic* pCharacteristic, message_t msgR, message_t msgY, char* msgBLE);

#endif