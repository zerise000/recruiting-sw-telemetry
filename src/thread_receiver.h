#ifndef __THREAD_RECIVER_H__
#define __THREAD_RECIVER_H__
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <queue>
#include <thread>

extern "C"{
  #include "fake_receiver.h"
}

typedef struct{
  char msg_buff[MAX_CAN_MESSAGE_SIZE];
  pthread_mutex_t lock;
  int nr_bytes;

}Receiver_args;

void start_receiver(std::queue<std::string> &msg_queue);

#endif
