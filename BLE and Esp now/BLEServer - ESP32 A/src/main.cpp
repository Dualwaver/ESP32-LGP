/**
  * @note código para o ESP32 amarelo
  *    
  * */
#include "util.h"
#include "EspNow_util.h"
#include "BLE_util.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

/*=================== 
 * Global Variables * 
 ===================*/

  // # LED
  const int ledPin = 23; //LED PIN
  message_t msgYellow; //Yellow led 
  message_t msgRed; //Red led

  // # ESP32 to peer macAddress
  const uint8_t peerAddress[] = {0x08, 0x3A, 0xF2, 0x8F, 0xA6, 0xBC};

  // # Bluetooth LE
  BLEServer *pServer;
  BLECharacteristic *pCharacteristic;
  bool deviceConnected = false;
  bool oldDeviceConnected = false;
  ServerCallbacks sCall;

  // # others
  int timer = 0;
  bool start = false;
  char value[256], BLEMsg[30];
  const char *lib[] = { "led on", "led off", "red on", "red off",
                        "yellow on", "yellow off", };

// End of Global Variables

/*======================= 
 * BLE auxiliar classes * 
 =======================*/ 
  //Characteristic callbacks
  class characteristicCallback:
    public BLECharacteristicCallbacks{
      void onWrite(BLECharacteristic *customCharac){
        std::string rcvStr = customCharac->getValue();
        if(rcvStr.length()>0){
          Serial.println("\nValue Received from BLE: ");
          memset(value,0,sizeof(value));
          for (int i = 0; i < rcvStr.length(); ++i)
            value[i]=rcvStr[i];
          start = true;
        }else
          Serial.println("Empty Value Received!");
      }
      void onRead(BLECharacteristic *customCharac){
        // ? supostamente isto não deve ser aqui
        if(msgRed.ledState == msgYellow.ledState)
          strcpy(BLEMsg,msgRed.ledState?"Red and Yellow ON":"Red and Yellow ON");
        else if(msgRed.ledState)
          strcpy(BLEMsg,"Red ON Yellow OFF");
        else
          strcpy(BLEMsg,"Red OFF Yellow ON");

        customCharac->setValue((char*)&BLEMsg);
      }
    };

//     //Server callbacks
//     class ServerCallbacks: public BLEServerCallbacks {
//     void onConnect(BLEServer* server) {
//       deviceConnected = true;
//     };

//     void onDisconnect(BLEServer* server) {
//       deviceConnected = false;
//     }
// };

// End of BLE auxiliar classes

/*======================== 
 * Functions declaration * 
 ========================*/ 
  
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len);

  int phoneMsg();

// End of function declaration

void setup(){
  Serial.begin(115200);
  
  // * ESP-NOW INIT 
  /* Init */
  initEspNow(OnDataReceive, OnDataSent);

  /* Create peer */
  //Creates & defines the peer address, comunication channel and encription
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //Add & verifies if the peer has been added to the peer list
  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // * BLE INIT
  BLEDevice::init("ESP32 Server");
  pServer = BLEDevice::createServer();
  //pServer->setCallbacks(new ServerCallbacks); //Set server callbacks
  pServer->setCallbacks(&sCall);
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE|
                                        BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->setCallbacks(new characteristicCallback);
  strcpy(BLEMsg,"Sem valores defenidos");
  pCharacteristic->setValue((char*)&BLEMsg);
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  /* Initial message */
  msgRed.ledState = false;
  //msg.dataSize = sizeof(msg);
  sendMsg(peerAddress,msgRed);

  /* Define pins */
  pinMode(ledPin, OUTPUT);// initialize digital pin ledPin as an output.
}

void loop() {
  if (start){
    int ph = phoneMsg();
    if (ph == -1)
      Serial.println("Invalid message");
    else
      switch (ph)
      {
      case 0:
        sendMsg(peerAddress,msgRed);
        digitalWrite(ledPin,msgYellow.ledState);
        break;

      case 1:
        sendMsg(peerAddress,msgRed);
        break;
      
      case 2:
        digitalWrite(ledPin,msgYellow.ledState);
        break;

      default:
        Serial.println("Error on message");
        break;
      }
    
    notifyBLE(pCharacteristic,msgRed,msgYellow,BLEMsg);
    start = false;
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

/*==================== 
 * ESP-NOW Functions * 
 ====================*/

  /**
  * @brief Callback function for when data is sent
  */
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.println("\nLast Packet Send Status:");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    Serial.println(msgRed.ledState == true ? "Red LED ON" : "Red LED OFF");
  }

  /**
  * @brief Callback function fo when data is received
  */

  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    memcpy(&msgYellow,data,sizeof(message));
    //memcpy(&ledState, data, sizeof(bool));
    Serial.println("\nLast Packet Received Content:");
    Serial.print("Bytes received: ");
    Serial.println(data_len);
    Serial.println(msgYellow.ledState == true ? "Yellow LED ON" : "Yellow LED OFF");
    digitalWrite(ledPin,msgYellow.ledState);
  }

// End of ESP-NOW Functions

/*===================== 
 * Auxiliar Functions * 
 =====================*/

  int phoneMsg(){
    int aux = 11;
    for (int i = 0; i < sizeof(lib)/sizeof(lib[0]); i++)
      if (strcmp(value,lib[i]) == 0){
        aux = i;
        break;
      }
    if (aux <= 1){
      if(aux == 0){
        msgRed.ledState = true;
        msgYellow.ledState = true;
      }else{
        msgRed.ledState = false;
        msgYellow.ledState = false;
      }
      aux = 0;
    } else if (aux <= 3){
      msgRed.ledState = aux == 2 ? true : false;
      aux = 1;
    } else if(aux <= 5){
      msgYellow.ledState = aux == 4 ? true : false;
      aux = 2;
    } else
      return -1;

    return aux;
  }

// End of BLE Functions