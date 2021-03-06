#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Normal UIID
#define SERVICE_UUID                    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define MINUTE_CHARACTERISTIC_UUID      "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SECOND_CHARACTERISTIC_UUID      "16d3930d-2523-4e25-bee8-86c84708b63c"

// 16bit short form UUID
//  #define SERVICE_UUID                    BLEUUID((uint16_t)0x1600)
// #define MINUTE_CHARACTERISTIC_UUID      BLEUUID((uint16_t)0x1A00)
// #define SECOND_CHARACTERISTIC_UUID      BLEUUID((uint16_t)0x1B00)


// #BLE Variables
BLEServer* server = NULL;
BLEService* service = NULL;
BLECharacteristic* minutes = NULL;
BLECharacteristic* seconds = NULL;
// BLEDescriptor* description = NULL;

bool deviceConnected = false; //*it informs the server if a device is connected
bool oldDeviceConnected = false;

char buffer[20],buf_min[10],buf_sec[10];//time, min and sec buffers

// #Server Callback class
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* server) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* server) {
      deviceConnected = false;
    }
};


int mint = 0, sec = 0, t = 0;
bool print = false;

void counter(){
  print = false;
  if ((millis()-t) == 1000){
    t = millis();
    sec++;
    print = true;
  }
  if (sec == 60){
    sec = 0;
    mint++;
  }
  if(millis() == 1000)
    t = millis();
  
  sprintf(buf_min, "%d min",mint);
  sprintf(buf_sec, "%d sec",sec);
  sprintf(buffer, "%s %s",buf_min,buf_sec);
}

void setup() {
  Serial.begin(115200);
  //Initialize the BLE
  BLEDevice::init("ESP32_test teste c");
  //Create the server
  server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks);
  //Create the service
  service = server->createService(SERVICE_UUID);
  //Create the min characteristic
  minutes = service->createCharacteristic(MINUTE_CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_NOTIFY|
                            BLECharacteristic::PROPERTY_INDICATE
                            );
  //add descriptor
  minutes->addDescriptor(new BLE2902());
  //Create the second characteristic
  seconds = service->createCharacteristic(SECOND_CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_NOTIFY |
                            BLECharacteristic::PROPERTY_INDICATE
                            );
  //add descriptor
  seconds->addDescriptor(new BLE2902());
  //start the service
  service->start();
  
  //todo: 
  //start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");

  Serial.println("\nTimer:");
}

/*
 #the objective is to notify the number of mins that have pass
 todo: notify the number of seconds
*/
void loop() {
  counter();
  if (print){
    Serial.print("time: ");
    Serial.println(buffer);
  }
  if(deviceConnected){
    if (print){
      minutes->setValue(buf_min);
      minutes->notify();
      seconds->setValue(buf_sec);
      seconds->notify();
    }
  }

  // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        server->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}