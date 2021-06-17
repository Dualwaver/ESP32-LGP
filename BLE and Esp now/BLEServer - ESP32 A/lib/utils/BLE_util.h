#ifndef BLE_UTIL_H_
#define BLE_UTIL_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "util.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void notifyBLE(BLECharacteristic* pCharacteristic, message msgR, message msgY, char* msgBLE);

//Server callbacks
class ServerCallbacks: public BLEServerCallbacks {    

    bool deviceConnected = false;
        
    void onConnect(BLEServer* server) {
        deviceConnected = true;
    };
  
    void onDisconnect(BLEServer* server) {
        deviceConnected = false;
    }

    bool getDeviceState(){
        return deviceConnected;
    }
};

#endif

void notifyBLE(BLECharacteristic* pCharacteristic, message msgR, message msgY, char* msgBLE);