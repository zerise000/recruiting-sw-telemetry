#include "src/thread_receiver.h"
#include "src/fsm.h"

extern "C"{
    #include "src/fake_receiver.h"
}


bool parse_msg(std::string msg,Message* parsed_msg){
  
    std::string str_id="0x";
    std::string str_payload="0x";

    uint16_t msg_len = msg.length();
    
	// in case main thread is trying to parse a message 
	// before receiver thread has got it then
	// return an error value
    if(msg_len-1 <= 0){
      return 1;
    }

    // check payload size, if not even then return error value
    if((msg_len-4) % 2 != 0){
       std::cerr << std::endl << "ERROR: invalid payload"; 
       return 1;
    }

	// the character at third pos is separator between ID and payload 
    for(size_t i=0; i<msg_len; i++){
		// if i<3 then we're parsing ID, otherwise start parsing the payload
		if(i<3)
			str_id += msg[i];
		else if(i>3)
			str_payload += msg[i];
    }


	// convert string ID and payload into hex integers
    parsed_msg->id = std::stoul(str_id,0,16);
    parsed_msg->payload = std::stoul(str_payload,0,16);

    return 0;
}

void main_thread(std::queue<std::string> &msg_queue,
		std::queue<Message> &msg_save_queue){
	
	Message parsed_msg;

    bool parse_res;
	std::string msg;
 
    do{
		// wait until you receive a message
		while(msg_queue.empty());

		// retrieve a message from the queue
		msg = msg_queue.front();
		msg_queue.pop();

		// parse the message, get the ID and the payload 
		parse_res = parse_msg(msg,&parsed_msg);
		
		// if parsing succeeded then update FSM
		if(!parse_res){
			msg_save_queue.push(parsed_msg);	
		}

    }while(msg.length() != 0);

	// tell to session_updater thread that there are no more messages to notify
	msg_save_queue.push({.id = 0,.payload = 0});
}


int main(void){

    open_can("candump.log");

	//we use a queue to send messages from one thread to another
	std::queue<std::string> msg_queue;
	std::queue<Message> msg_save_queue;

	// start threads 
	std::thread receiver(start_receiver,std::ref(msg_queue));
	std::thread session_updater(save_msg,std::ref(msg_save_queue));


    main_thread(std::ref(msg_queue),std::ref(msg_save_queue));

	// wait the end of receiver and session_updater, then close the program
	receiver.join(); 
	session_updater.join();
    close_can();

    return 0;
}
