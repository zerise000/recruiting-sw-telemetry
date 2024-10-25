#include "thread_receiver.h"


void start_receiver(std::queue<std::string> &msg_queue){
  char received_msg[MAX_CAN_MESSAGE_SIZE];
  std::string msg_wrapper="";
  int nr_bytes = 0;

  do{
	//reset message buffer	
	memset(received_msg,0,MAX_CAN_MESSAGE_SIZE);

	//read message
	nr_bytes = can_receive(received_msg);

	// set std::string wrapper and send it
	msg_wrapper = received_msg;
	msg_queue.push(msg_wrapper);

  }while(nr_bytes != -1);

}
