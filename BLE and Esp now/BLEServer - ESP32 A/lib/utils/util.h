#ifndef UTIL_H_
#define UTIL_H_

typedef struct message{
  int numb;
  char text[20];  
  bool ledState;  //  -> ESP led state
  //int dataSize;     -> Check the size of data sent
} message_t;

#endif