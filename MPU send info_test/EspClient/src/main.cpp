/**
  * @note código para o ESP32 preto
  *    
  * */

#include <Arduino.h>
#include "../lib/utils/EspNow_util.h"
//todo: create a struct that contains the sensor information so its easier to pass info

/*=================== 
 * Global Variables * 
 ===================*/
  // # ESP32 to peer MAC address
  const uint8_t peerAddress[] = {0xAC, 0x67, 0xB2, 0x2B, 0xDA, 0x60}; 

  // #others
  bool print = false;
  int time;

// End of Global Variables

/*======================== 
 * Functions declaration * 
 ========================*/ 
  
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len);

// End of function declaration

void setup() {
  Serial.begin(115200);
  /* Init */
  initEspNow(OnDataReceive,NULL);

  /* Create peer */
  //todo: tentar encapsular esta parte numa função. @brief Creates & defines the peer address, comunication channel and encription
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  /* Add peer */
  if(esp_now_add_peer(&peerInfo) != ESP_OK){ //Add & verifies if the peer has been added to the peer list
    Serial.println("Failed to add peer");
    return;
  }  
}

void loop(){
  pid_t retfork = fork();
  if (retfork = 0){
    Serial.println("Esta porcaria funciona\n");

  }
  
}

/*============ 
 * Functions * 
 ============*/

  /**
  * @brief Callback function fo when data is received
  */
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    message_t rcvData;
    memcpy(&rcvData,data,sizeof(rcvData));
    Serial.println("\nLast Packet Received Content:");
    for( int i = 0; i < rcvData.; i++)
    {
      /* code */
    }
    
    Serial.print("MPU:");
    Serial.println(rcvData.finger);
    Serial.print(rcvData.ax,4);
    Serial.print(",");
    Serial.print(rcvData.ay,4);
    Serial.print(",");
    Serial.println(rcvData.az,4);
    Serial.println("");
    Serial.print("Bytes received: ");
    Serial.println(data_len);
  }

// End of Functions