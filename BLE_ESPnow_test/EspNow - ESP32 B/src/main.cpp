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
  // # LED
  const int ledPin = 23; //LED PIN
  message_t msg;  

  // # BUTTON
  const int buttonPin = 4; //BUTTON PIN
  bool buttonState; //BUTTON state
  bool buttonPress = false;

  // # Peer LED
  bool peerLED = false;

  // # ESP32 to peer MAC address
  const uint8_t peerAddress[] = {0xAC, 0x67, 0xB2, 0x2B, 0xDA, 0x60};


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
  initEspNow(OnDataReceive,OnDataSent);
  Serial.print("\n Msg size:");
  Serial.println(sizeof(msg));

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

  /* Initial message */
  msg.ledState = false;
  //msg.dataSize = sizeof(msg);
  sendMsg(peerAddress,msg);

  /* Define pins */
  pinMode(ledPin, OUTPUT); // initialize digital pin ledPin as an output.
  pinMode(buttonPin, INPUT); // initialize digital pin ButtonPin as an input.
}

void loop() {
  buttonState = digitalRead(buttonPin);
   
  if (buttonState && !buttonPress){
    //digitalWrite(ledPin,msg.ledState);
    //todo: when button press send message to peer turning on/off the peer's LED pass the peerLED var to peer
    sendMsg(peerAddress, msg);
    delay(100);
    //peerLED = !peerLED; // Change the peers led
    msg.ledState = !msg.ledState;
    buttonPress = true;
  }else if (!buttonState)
    buttonPress = false;
}

/*============ 
 * Functions * 
 ============*/

  /**
  * @brief Callback function fo when data is sent
  */
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.println("\nLast Packet Send Status:");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    Serial.println(msg.ledState ? "Yellow LED ON" : "Yellow LED OFF");
  }

  /**
  * @brief Callback function fo when data is received
  */
  void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    struct message recvMsg;
    memcpy(&recvMsg,data,sizeof(recvMsg));
    // memcpy(&ledState, data, sizeof(bool));
    Serial.println("\nLast Packet Received Content:");
    Serial.print("Bytes received: ");
    Serial.println(data_len);
    Serial.println(recvMsg.ledState ? "Red LED ON" : "Red LED OFF");
    digitalWrite(ledPin,recvMsg.ledState);
  }

// End of Functions