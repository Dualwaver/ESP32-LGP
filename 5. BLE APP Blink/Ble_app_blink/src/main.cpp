#include <Arduino.h>
#include "../lib/utils/BLE_util.h"
#include "../lib/utils/EspNow_util.h"
//todo: create a struct that contains the sensor information so its easier to pass info

/*=================== 
 * Global Variables * 
 ===================*/
  // # ESP32 to peer MAC address
  uint8_t peerAddress[] = {0x3C, 0x61, 0x05, 0x13, 0x70, 0x20}; //->ESPPreto Vasco

  // #BLE
  BLEServer *pServer;
  BLECharacteristic *pLedChange;//, *pCharX, *pCharY, *pCharZ;

  // #Connection Variables
  bool deviceConnected = false;
  bool oldDeviceConnected = false;

  // #others
  int ledPin = 2; // LED GPIO2
  bool ledState = false;
  bool changeState = false;

// End of Global Variables

/*=================== 
 * Auxiliar Classes * 
 ===================*/
  //Server callbacks
  class ServerCallbacks: public BLEServerCallbacks {    
          
    void onConnect(BLEServer* server) {
      Serial.println("Device Connected");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* server) {
      Serial.println("Device Disconnected");
        deviceConnected = false;
    }
  };

  //Characteristics callbacks
  class CharactCallbacks: public BLECharacteristicCallbacks{
    void onRead(BLECharacteristic* pCharacteristic){
      // do stuff here on read
      Serial.println("Characteristic has been read\n");
    }

	  void onWrite(BLECharacteristic* pCharacteristic){
      // do stuff here on write
      Serial.println("Characteristic has been writen");
      std::string rcvString = pCharacteristic->getValue();
      Serial.println(rcvString.c_str());
      strcmp(rcvString.c_str(),"on") == 0 ? ledState = true : ledState = false;
      changeState = true;  
    }
  };
  
// End of Auxiliar Classes

/*======================== 
 * Functions declaration * 
 ========================*/ 

// End of function declaration

void setup() {
  Serial.begin(115200);

  /* Create peer */
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  //todo: create a function that creates BLE Device and Server
  // Create the BLE Device
  BLEDevice::init("ESP32 blink");

  //Create Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  //Create Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Create characteristic
  pLedChange = pService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_WRITE|
                                                                   BLECharacteristic::PROPERTY_READ);
  pLedChange->addDescriptor(new BLE2902());
  pLedChange->setCallbacks(new CharactCallbacks());
  pLedChange->setValue((char*)"off");

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();

  Serial.println("\nWaiting a client connection...");

  pinMode(ledPin,OUTPUT);
  Serial.print("Led state: ");
  Serial.println(ledState ? "on\n" : "off\n");
  
}

void loop(){

  if(deviceConnected  && changeState){
    Serial.print("Led state: ");
    Serial.println(ledState ? "on\n" : "off\n");
    digitalWrite(ledPin,ledState);
    changeState = false;
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising...");
    digitalWrite(ledPin,LOW);
    Serial.println("Led state: off\n");
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

// End of Functions