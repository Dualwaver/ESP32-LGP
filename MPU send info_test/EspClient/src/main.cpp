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
  uint8_t peerAddress[] = {0x3C, 0x61, 0x05, 0x13, 0x70, 0x20}; //->ESPPreto Vasco
  //const uint8_t peerAddress[] = {0xAC, 0x67, 0xB2, 0x2B, 0xDA, 0x60}; //-> ESPAmarelo

  // #others
  bool print = false;
  int t = 0;

// End of Global Variables

/*======================== 
 * Functions declaration * 
 ========================*/ 
  
  //void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len);

  String handDescreminator(location_t local);

// End of function declaration

void setup() {
  Serial.begin(115200);
  /* Init */
  initEspNow(OnDataReceive,NULL);

  /* Create peer */
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr,peerAddress,sizeof(peerAddress)/sizeof(peerAddress[0]));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  /* Add peer */
  // esp_err_t retval = esp_now_add_peer(&peerInfo);//Add & verifies if the peer has been added to the peer list
  // if(retval != ESP_OK){ 
  //   Serial.print("ERROR: Failed to add peer ->");
  //   Serial.println(esp_err_to_name(retval));
  //   esp_now_del_peer(peerAddress);
  //   // Serial.println(peerInfo.channel);
  //   // esp_now_deinit();
  //   return;
  // }  
  Serial.println(WiFi.macAddress());
  Serial.println("Comunication start");
}

void loop(){
  delay(100);
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
    Serial.print("\nLast Packet Received Content:\n");
    //for( int i = 0; i < 3; i++){
    Serial.print("MPU:");
    Serial.print(rcvData.x,4);
    Serial.print(",");
    Serial.print(rcvData.y,4);
    Serial.print(",");
    Serial.println(rcvData.z,4);
    Serial.print("Dedo: ");
    Serial.println(handDescreminator(rcvData.local));
    
    Serial.print("Bytes received: ");
    Serial.println(data_len);
  }

  // void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len){
  //   int rcvData;
  //   memcpy(&rcvData,data,sizeof(rcvData));
  //   Serial.print("Valor recebido:");
  //   Serial.println(rcvData);
  // }

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
      str = "Mendinho";
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