
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
  BLECharacteristic *pCharSensor;//, *pCharX, *pCharY, *pCharZ;
  uint8_t send[BLE_SEND_SIZE];

  // #Connection Variables
  bool deviceConnected = false;
  bool oldDeviceConnected = false;

  // #Message
  message_t msg;

  // #others
  int value = 0;
  char str[50];
  //bool print = false;
  bool print = true; // apagar

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
      Serial.println("I read");
    }

	  void onWrite(BLECharacteristic* pCharacteristic){
      // do stuff here on write
      Serial.println("I wrote");
    }
  };
  
// End of Auxiliar Classes

/*======================== 
 * Functions declaration * 
 ========================*/ 

  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len);

  String handDescreminator(location_t local);

  void messageToUint8 (message_t *message, uint8_t *send, uint8_t hand);

// End of function declaration

void setup() {
  Serial.begin(115200);
  
  //ESPNOW

  //initEspNow(OnDataReceive,NULL);

  /* Create peer */
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  //todo: create a function that creates BLE Device and Server
  // Create the BLE Device
  BLEDevice::init("ESP32 test");

  //Create Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  //Create Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Create characteristic
  pCharSensor = pService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_NOTIFY);
  pCharSensor->addDescriptor(new BLE2902());
  // pCharX = pService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_NOTIFY);
  // pCharX->addDescriptor(new BLE2902());
  // pCharY = pService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_NOTIFY);
  // pCharY->addDescriptor(new BLE2902());
  // pCharZ = pService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_NOTIFY);
  // pCharZ->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();

  msg.local = THUMB;
  msg.x = -1.1340f;
  msg.y = -0.6167f;
  msg.z =  9.1422f;

  Serial.println("\nWaiting a client connection to notify...");

  Serial.println("Info na mensagem enviada:");
  Serial.print(UINT8_MAX,HEX); Serial.print(": ");
  Serial.print(msg.local,HEX); Serial.print(": ");
  Serial.print(msg.x,HEX); Serial.print(": ");
  Serial.print(msg.y,HEX); Serial.print(": ");
  Serial.println(msg.z,HEX);

messageToUint8 (&msg, send, 0xFF);

  Serial.println("Info enviada:");
  for (size_t i = 0; i < BLE_SEND_SIZE; i++){
    if(i < (BLE_SEND_SIZE - 1)){
      Serial.print(send[i],HEX); Serial.print(": ");
    }else
      Serial.println(send[i],HEX);
  }
  
}

void loop(){
  //notify changed value
  // if (deviceConnected & print) {
  //     //sprintf(str, "%s: %f %f %f",handDescreminator(msg.local).c_str(), msg.x, msg.y, msg.z);
  //     //sprintf(str, "%d: %0.4f %0.4f %0.4f",msg.local, msg.x, msg.y, msg.z);
  //     //sprintf(str, "%0.4f %0.4f %0.4f", msg.x, msg.y, msg.z);
  //     //pCharSensor->setValue((uint8_t *)str, sizeof(str)/sizeof(str[0]));
  //     pCharSensor->setValue(handDescreminator(msg.local).c_str());
  //     sprintf(str, "x: %0.4f", msg.x);
  //     pCharX->setValue(str);
  //     sprintf(str, "y: %0.4f", msg.y);
  //     pCharY->setValue(str);
  //     sprintf(str, "z: %0.4f", msg.z);
  //     pCharZ->setValue(str);
  //     pCharSensor->notify();
  //     delay(50);
  //     pCharX->notify();
  //     delay(50);
  //     pCharY->notify();
  //     delay(50);
  //     pCharZ->notify();
  //     value++;
  //     delay(50); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  //     print = false;
  // }

  if(deviceConnected & print){
    messageToUint8 (&msg, send, 0XFF);
    pCharSensor->setValue((uint8_t *) send, sizeof(send)/sizeof(send[0]));
    pCharSensor->notify();
    delay(50);
    //print = false;
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

    /**
  * @brief Callback function fo when data is received
  */
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    //message_t rcvData;
    if(!print){
      memcpy(&msg,data,sizeof(msg));
      Serial.print("\nLast Packet Received Content:\n");
      //for( int i = 0; i < 3; i++){
      Serial.print("MPU:");
      Serial.print(msg.x,4);
      Serial.print(",");
      Serial.print(msg.y,4);
      Serial.print(",");
      Serial.println(msg.z,4);
      Serial.print("Dedo: ");
      Serial.println(handDescreminator(msg.local));
      
      Serial.print("Bytes received: ");
      Serial.println(data_len);

      print = true;
    }
  }

  String handDescreminator(location_t local){
    String str;
    //String str = (char *) malloc(sizeof("Mão-Acelarometro"));
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

// End of Functions