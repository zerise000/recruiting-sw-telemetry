#include "src/thread_receiver.h"
#include "src/fsm.h"

extern "C"{
    #include "src/fake_receiver.h"
}


bool parse_msg(std::string msg,uint16_t* id,uint64_t* payload){
  
    std::string str_id="";
    std::string str_payload="";

    uint16_t msg_len = msg.length();
    
    if(msg_len-1 <= 0){
      return 1;
    }

    // check payload size, if not even then return error
    if((msg_len-4) % 2 != 0){
       std::cerr << std::endl << "ERROR: invalid payload"; 
       return 1;
    }

    for(size_t i=0; i<msg_len; i++){
      if(i<3)
	str_id += msg[i];
      else if(i>3)
	str_payload += msg[i];
    }


    *id = std::stoul(str_id,nullptr,16);
    *payload = std::stoul(str_payload,nullptr,16);
   
    return 0;
}

void main_thread(std::queue<std::string> &msg_queue){

    uint16_t id=0;
    uint64_t payload=0;
    bool parse_res;
	std::string msg;
 
    do{
		while(msg_queue.empty());

		msg = msg_queue.front();
		msg_queue.pop();

		parse_res = parse_msg(msg,&id,&payload);
		
		if(!parse_res){
			next_state(payload);
			update_session(id,payload);
		}

    }while(msg.length() != 0);

}


int main(void){

    open_can("candump.log");
	std::queue<std::string> msg_queue;


	std::thread receiver(start_receiver,std::ref(msg_queue));

    main_thread(std::ref(msg_queue));
	receiver.join(); 
    close_can();

    return 0;
}
