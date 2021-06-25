#ifndef UTIL_H_
#define UTIL_H_

typedef enum{ 
  THUMB, 
  INDEX, 
  MIDDLE, 
  RING, 
  PINKY,
  HAND_A,
  HAND_G
} location_t;

// #define FINGER_THUMB    0   /* dedo pulgar */
// #define FINGER_INDEX    1   /* dedo indicador */
// #define FINGER_MIDDLE   2   /* dedo do meio */
// #define FINGER_RING     3   /* dedo anelar */
// #define FINGER_PINKY    4   /* dedo mindinho */

typedef struct messsage{
  float x;
  float y;
  float z;
  location_t local;
}message_t;


#endif
