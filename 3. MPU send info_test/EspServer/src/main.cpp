#include <Arduino.h>
#include "WiFi.h"
#include "../lib/utils/EspNow_util.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define N_MPU 3     //number of MPU6050 connected to the same i2c bus

Adafruit_MPU6050 mpu[N_MPU];

boolean prt;
int i=0, j=0;

//indice de mpu
#define S0 12
#define S1 13
#define S2 14

int mpu_index = 1;

//sensor events
sensors_event_t a, g, t;

// # ESP32 to peer MAC address
  const uint8_t peerAddress[] = {0x08, 0x3A, 0xF2, 0x8F, 0xA6, 0xBC};
  //const uint8_t peerAddress[] = {0xAC,0x67, 0xB2, 0x2B, 0xDA, 0x60};

//create fingerData
  message_t *fing = (message_t*) malloc(sizeof(message_t));

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.println("\nLast Packet Send Status:");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //espnow setup
  initEspNow(NULL,OnDataSent);
  /* Create peer */
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  
  // Try to initialize!
  for(i=0; i<N_MPU ;i++){
    switch(i)
  {
    case 0:
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW); 
      break;
    case 1:
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW);
      break;
    case 2:
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,LOW);
    default:
      break;  
  }
    delay(100);
    while(!mpu[i].begin(0x69)){
      Serial.print("Failed to find MPU6050 chip nº");
      Serial.println(i);
      delay(10);
    }  
  }
  //selecionar os ranges e filtro
  for(i=0; i<N_MPU ;i++){
    mpu[i].setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu[i].setGyroRange(MPU6050_RANGE_500_DEG);
    mpu[i].setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  
  Serial.println("--------------Loop start--------------");
  Serial.println();
  digitalWrite(S0,LOW);
  digitalWrite(S1,LOW);
  digitalWrite(S2,LOW);
}


void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("Mac:");
  // Serial.println(WiFi.macAddress());
  Serial.print("MPU nº ");
  Serial.println(mpu_index); 


  switch(mpu_index)
  {
    case 1:
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      // digitalWrite(S2,LOW); 
      delay(10);
      fing->local = THUMB;
      mpu[0].getEvent(&a, NULL, NULL);
      break;
    case 2:
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      // digitalWrite(S2,LOW);
      delay(10);
      fing->local = INDEX;
      mpu[1].getEvent(&a, &g, &t);
      break;
    case 3:
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      // digitalWrite(S2,LOW);
      delay(10);
      fing->local = MIDDLE;
      mpu[2].getEvent(&a, &g, &t);
      break;
    default:
      break;  
  }

  /* Print acelerómetro */
  Serial.print(a.acceleration.x,4);
  Serial.print(",");
  Serial.print(a.acceleration.y,4);
  Serial.print(",");
  Serial.print(a.acceleration.z,4);
  Serial.print(", ");
  Serial.print("Dedo: ");
  Serial.print(fing->local);

  fing->x = a.acceleration.x;
  fing->y = a.acceleration.y;
  fing->z = a.acceleration.z;


  delay(3000);
  sendMsg(peerAddress,fing);
  delay(10);
  Serial.println("");

    
  
  // /* Print giroscópio */
  // /*
  // Serial.print(g.gyro.x,4);
  // Serial.print(",");
  // Serial.print(g.gyro.y,4);
  // Serial.print(",");
  // Serial.print(g.gyro.z,4);
  // */
  // Serial.println("");

  if(mpu_index < N_MPU) mpu_index++;
  else mpu_index = 1;
  
  //delay(100);
}