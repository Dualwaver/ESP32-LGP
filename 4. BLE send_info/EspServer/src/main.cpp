
#include <Arduino.h>
#include "../lib/utils/BLE_util.h"
//todo: create a struct that contains the sensor information so its easier to pass info

/*=================== 
 * Global Variables * 
 ===================*/
  // #BLE
  BLEServer *pServer;
  ServerCallbacks sCallback;
  BLECharacteristic *pCharSensor;
  //BLEDescriptor *d;

  // #others
  bool print = false;
  int t = 0;

// End of Global Variables

/*======================== 
 * Functions declaration * 
 ========================*/ 

  String handDescreminator(location_t local);

// End of function declaration

void setup() {
  Serial.begin(115200);

  //Create Server
  pServer->setCallbacks(&sCallback);
  pServer = BLEDevice::createServer();
  
  //Create Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Create characteristic
  pCharSensor = pService->createCharacteristic(CHARACTERISTIC_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
                BLECharacteristic::PROPERTY_NOTIFY| BLECharacteristic::PROPERTY_INDICATE);


}

void loop(){
  if (sCallback deviceConnected) {
      pCharacteristic->setValue((uint8_t*)&value, 4);
      pCharacteristic->notify();
      value++;
      delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
    // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
    // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}

/*============ 
 * Functions * 
 ============*/

  String handDescreminator(location_t local){
    String str;
    switch (local){
    case THUMB:
      str = "Pulgar";
      break;
    case INDEX:
      str = "Indicador";
      break;
    case MIDDLE:
      str = "Dedo do meio";
      break;
    case RING:
      str = "Anelar";
      break;
    case PINKY:
      str = "Mindinho";
      break;
    case HAND_A:
      str = "Mão-Acelarometro";
      break;
    case HAND_G:
      str = "Mão-Giroscópio";
      break;
    default:
      break;
    }

    return str;
  }

// End of Functions