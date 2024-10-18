#include "thread_receiver.h"


void* start_receiver(void* args){
  Receiver_args* local_args = (Receiver_args*)args;
  
  while(1){
    pthread_mutex_lock(&(local_args->lock));
    
    pthread_mutex_unlock(&(local_args->lock));
  }
}
