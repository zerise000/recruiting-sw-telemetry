#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <ctime>
#include <cstring>
#include "thread_receiver.h"

extern "C"{
    #include "fake_receiver.h"
}


int main(void){
  bool state=false; //initial state is IDLE

  
  Receiver_args* r_args;
  pthread_t receiver;
  
  std::time_t timestamp = std::time(nullptr);
  std::string filename;
  
  
  if(pthread_mutex_init(&r_args->lock,NULL)){
    std::cout << "lock creation failed";
    return 1;
  }
  
  pthread_create(&receiver,NULL,start_receiver,(void*)r_args);
  

  while(1){
    pthread_mutex_lock(&(r_args->lock));

    if(std::strlen(r_args->msg_buff) > 0){
	if(!state){ //IDLE

	  filename = std::asctime(std::localtime(&timestamp));
	  open_can(filename.c_str());

	}else{ //RUN
	}
    }
    pthread_mutex_unlock(&(r_args->lock));
  }


  pthread_mutex_destroy(&(r_args->lock));
  
  return 0;
}
