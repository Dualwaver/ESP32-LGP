#include "EspNow_util.h"
#include "util.h"

/**
  * @brief Initializes the ESP-NOW protocol and assign the callback functions
  * @param rcb Callback function of receiving ESPNOW data
  * @param scb Callback function of sending ESPNOW data
  */
void initEspNow(esp_now_recv_cb_t rcb, esp_now_send_cb_t scb){
  //ESP-NOW uses wifi interface so it is initialized
  WiFi.mode(WIFI_MODE_STA);
      
  //Initializes and verifies if the esp now interface has been initialized successfully
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  if(rcb != NULL)//assign the calback function received
    esp_now_register_recv_cb(rcb);

  if(scb != NULL)//assign the calback function sent
    esp_now_register_send_cb(scb);  
}

/**
  * @brief Sends a message to the known peer
  * @param peer_addr peer MAC address
  * @param msg message_t data struct to be sent
*/
void sendMsg(const uint8_t *peer_addr, message_t msg){
  if (esp_now_send(peer_addr,(uint8_t *) &msg,sizeof(msg)) != ESP_OK)
    Serial.println("\nError sending the data");
  else
    Serial.println("\nSent with Success");
}