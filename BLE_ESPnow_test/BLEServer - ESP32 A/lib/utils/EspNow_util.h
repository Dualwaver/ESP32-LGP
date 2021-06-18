#ifndef ESPNOW_UTIL_H_
#define ESPNOW_UTIL_H_

#include <esp_now.h>
#include <WiFi.h>
#include "util.h"

/**
  * @brief Initializes the ESP-NOW protocol and assign the callback functions
  * @param rcb Callback function of receiving ESPNOW data
  * @param scb Callback function of sending ESPNOW data
  */
void initEspNow(esp_now_recv_cb_t rcb, esp_now_send_cb_t scb);

/**
  * @brief Sends a message to the known peer
  * @param peer_addr peer MAC address
  * @param msg data struct to be sent
*/
void sendMsg(const uint8_t *peer_addr, struct message msg);

#endif