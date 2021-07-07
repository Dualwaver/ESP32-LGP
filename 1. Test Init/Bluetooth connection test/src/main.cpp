#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// #server address
#define address "08:3A:F2:8F:AG:BE"

// #BLE Variables
BLEClient* client = NULL;
BLERemoteService* service = NULL;
BLERemoteCharacteristic* minutes = NULL;
BLERemoteCharacteristic* seconds = NULL;
// BLEDescriptor* description = NULL;

bool deviceConnected = false; //*it informs the server if a device is connected
bool oldDeviceConnected = false;

char buffer[20],buf_min[10],buf_sec[10];//time, min and sec buffers


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
  BLEDevice::init("ESP32_adress");
  
}

/*
 #the objective is to notify the number of mins that have pass
 todo: notify the number of seconds
*/
void loop() {
  counter();
  // if (print){
  //   Serial.print("time: ");
  //   Serial.println(buffer);
  // }
  // if(deviceConnected){
  //   if (print){
  //     minutes->setValue(buf_min);
  //     minutes->notify();
  //     seconds->setValue(buf_sec);
  //     seconds->notify();
  //   }
  // }

  // // disconnecting
  //   if (!deviceConnected && oldDeviceConnected) {
  //       delay(500); // give the bluetooth stack the chance to get things ready
  //       server->startAdvertising(); // restart advertising
  //       Serial.println("start advertising");
  //       oldDeviceConnected = deviceConnected;
  //   }
  //   // connecting
  //   if (deviceConnected && !oldDeviceConnected) {
  //       // do stuff here on connecting
  //       oldDeviceConnected = deviceConnected;
  //   }
}