#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Wire.h"

#define LED                2
#define NUMB_OF_SENSORS    1
#define BLE_SEND_SIZE      sizeof(float)*3*NUMB_OF_SENSORS + NUMB_OF_SENSORS
                          //Sensor information(a.x,a.u,a.z) + sensor position and location description
#define CUSTOM_SERVICE     "544b335f-aa69-4be8-aef4-ca32a9867832"
#define LED_CHARACTERISTIC "e8695a15-7f64-4432-89af-2e42054399bb"
#define MPU_CHARACTERISTIC "27e57d1e-3941-4045-97c5-dbb7981a4434"
#define TCAADDR 0x70

Adafruit_MPU6050 mpu;

//sensor events
sensors_event_t a, g;

boolean connected, advert;

TwoWire bus1 = TwoWire(0);


void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  bus1.beginTransmission(TCAADDR);
  bus1.write(1 << i);
  bus1.endTransmission();  
}

/***** Array of MPU data to send ****/
uint8_t *send = (uint8_t*) malloc(sizeof(float)*3);


/****************  Função para passar floats para bytes   ********************/
void messageToUint8 (sensors_event_t *acl, uint8_t *send){
  memset(send,0,BLE_SEND_SIZE); // make sure the array is clear
  uint8_t val = 0xFF;
  for (size_t i = 0; i < BLE_SEND_SIZE; i += 4){
    if (i < 4)
      memcpy(&send[i],&acl->acceleration.x,sizeof(float));
    else if(i < 8)
      memcpy(&send[i],&acl->acceleration.y,sizeof(float));
    else if(i < 12)
      memcpy(&send[i],&acl->acceleration.z,sizeof(float));
    else
      memcpy(&send[i],&val,sizeof(uint8_t));  
  }
  Serial.printf("Message Sent:\n");
  for (size_t i = 0; i < BLE_SEND_SIZE; i++)
    i == BLE_SEND_SIZE - 1 ? Serial.printf("%X\n",send[i]) : Serial.printf("%X-", send[i]); 
}

BLEServer *pServer;
BLECharacteristic *ledCharacteristic, *mpuCharacteristic;

bool deviceConnected = false;

char value[50];
char text[50] = "Hello world from ESP32";


class ServerCallback : public BLEServerCallbacks{
  void onConnect(BLEServer* pServer){
    Serial.println("I'm connected boyyyyyyyyyyyy");
    connected = true;
  }
  void onDisconnect(BLEServer* pServer){
    Serial.println("I'm disconnected :(");
    connected = false;
    advert = true;
  }
};


class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *customCharacteristic){
    std::string str = customCharacteristic->getValue();

    if( strcmp(str.c_str(),"on") == 0 )
      digitalWrite(LED, HIGH);
    else digitalWrite(LED, LOW);
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);

  bus1.begin(21,22,100000);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();

  pServer->setCallbacks(new ServerCallback);

  // Create the BLE Service
  BLEService *pService = pServer->createService(CUSTOM_SERVICE);

  //Create the BLE Characteristic for LED
  ledCharacteristic = pService->createCharacteristic(LED_CHARACTERISTIC, BLECharacteristic::PROPERTY_WRITE);
  
  //Create the BLE Characteristic for MPU6050 and descriptor to allow notifications
  mpuCharacteristic = pService->createCharacteristic(MPU_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY);

  mpuCharacteristic->addDescriptor(new BLE2902());

  // add a BLE Characteristics
  pService->addCharacteristic(ledCharacteristic);
  pService->addCharacteristic(mpuCharacteristic);

  ledCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pServer->getAdvertising()->addServiceUUID(CUSTOM_SERVICE);

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  /**** Setup do MPU ****/
  //tcaselect(4);
  while(!mpu.begin(0x69)){
      Serial.println("Failed to find MPU6050 chip");
      delay(10);
  } 

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

void loop() {
  //tcaselect(4);
  mpu.getEvent(&a, &g, NULL);
  
  /* Print acelerómetro */
  Serial.println("\nM'PU:");
  Serial.print(a.acceleration.x,4);
  Serial.print(",");
  Serial.print(a.acceleration.y,4);
  Serial.print(",");
  Serial.println(a.acceleration.z,4);
  //Serial.print("\n");

  messageToUint8(&a, send);
  //delay(500);
  mpuCharacteristic->setValue( send, BLE_SEND_SIZE);
  mpuCharacteristic->notify();

  if(advert && !connected) {
    delay(500); //tempo para a stack ficar preparada
    pServer->startAdvertising();
    advert = false;
  }

  delay(2000);
}