#ifndef UTIL_H_
#define UTIL_H_

typedef struct message{
  handData_t hand;
  fingerData_t finger[5];
} message_t;

typedef enum{ 
  THUMB, 
  INDEX, 
  MIDDLE, 
  RING, 
  PINKY
} finger_t;

// #define FINGER_THUMB    0   /* dedo pulgar */
// #define FINGER_INDEX    1   /* dedo indicador */
// #define FINGER_MIDDLE   2   /* dedo do meio */
// #define FINGER_RING     3   /* dedo anelar */
// #define FINGER_PINKY    4   /* dedo mindinho */

typedef struct fingerData{
  float ax;
  float ay;
  float az;
  finger_t finger;
}fingerData_t;

typedef struct handSens{
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
}handData_t;


#endif