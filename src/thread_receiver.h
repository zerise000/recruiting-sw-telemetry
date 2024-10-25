#ifndef __THREAD_RECIVER_H__
#define __THREAD_RECIVER_H__
#include <iostream>
#include <queue>
#include <thread>
#include <cstring>

extern "C"{
  #include "fake_receiver.h"
}


void start_receiver(std::queue<std::string> &msg_queue);

#endif
