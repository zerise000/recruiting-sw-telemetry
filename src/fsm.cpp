#include "fsm.h"

//since the fsm has only two states we can use a boolean variable to represent them. 
//If variable is false then we're in IDLE state
static bool state = false;


void next_state(Session* curr_session,Message msg_in){

	// check transactions
	bool check_stop = msg_in.payload == 0x66FF and state;
  	bool check_start = (msg_in.payload == 0x6601 or msg_in.payload == 0xFF01) and (not state);
  
	// if we're in RUN state then save msg into log
	if(state)
	  curr_session->update_session(msg_in);	

	//exec transactions
	if(check_start){
	  curr_session->start_new_session();
	}else if(check_stop){
	  curr_session->stop_session();
	}

	// in case of transaction save it into log file and update state
	if(check_start or check_stop){
	  curr_session->update_session(msg_in);	
	  state = not state;
	}


}

void save_msg(std::queue<Message> &msg_ch){
	Message msg_in;
	Session curr_session;

	curr_session.id = 0;
	bool cond_loop = true;

	do{
		while(msg_ch.empty());

		msg_in = msg_ch.front();
		msg_ch.pop();
		cond_loop = msg_in.id != 0 or msg_in.payload != 0;

		if(cond_loop)
			next_state(&curr_session,msg_in);

	}while(cond_loop);

	curr_session.stop_session();
}


