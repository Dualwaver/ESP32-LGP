#ifndef UTIL_H_
#define UTIL_H_

typedef enum{ 
  THUMB,          /* dedo pulgar */
  INDEX,          /* dedo indicador */
  MIDDLE,         /* dedo do meio */
  RING,           /* dedo anelar */
  PINKY,          /* dedo mindinho */
  HAND_A,         /* mão acelarómetro */
  HAND_G          /* mão giroscópio */
} location_t;

typedef struct messsage{
  float x;
  float y;
  float z;
  location_t local;
}message_t;


#endif
