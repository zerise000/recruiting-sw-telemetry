#ifndef __THREAD_RECIVER_H__
#define __THREAD_RECIVER_H__
#include <pthread.h>

extern "C"{
  #include "fake_receiver.h"
}

typedef struct{
  char msg_buff[MAX_CAN_MESSAGE_SIZE];
  pthread_mutex_t lock;

}Receiver_args;

void* start_receiver(void* args);

#endif
