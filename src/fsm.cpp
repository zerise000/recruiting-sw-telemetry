#include "fsm.h"


void next_state(Fsm* fsm_in,Message msg_in){

	// check transactions
	bool check_stop = msg_in.payload == 0x66FF;
  	bool check_start = (msg_in.payload == 0x6601 or msg_in.payload == 0xFF01);
  
	// third digit: current state
	// second digit: transaction to RUN
	// first digit: transaction to IDLE
	uint8_t operation = check_stop + (check_start << 1) + (fsm_in->state << 2);

	switch(operation){
		case 0x4: //100 or 110
		case 0x6:
			// we're in RUN state, save msg into log
			(fsm_in->curr_session)->update_session(msg_in);
			break;
		case 0x5: // 101
			// we move from RUN to IDLE, close session and update state
			UPDATE(fsm_in);	
	  		delete (fsm_in->curr_session);
			break;
		case 0x2: // 010
			// we move from IDLE to RUN, open new session and save log start message
			fsm_in->curr_session = new Session();
			UPDATE(fsm_in);	
			break;
		default:
			// in all othre cases don't do nothing
			break;
	}

}

void save_msg(std::queue<Message> &msg_ch){
	Message msg_in;

	bool cond_loop = true;
	Fsm fsm_in;

	do{
		while(msg_ch.empty());

		msg_in = msg_ch.front();
		msg_ch.pop();
		cond_loop = msg_in.id != 0 or msg_in.payload != 0;

		if(cond_loop)
			next_state(&fsm_in,msg_in);

	}while(cond_loop);

	if(fsm_in.curr_session != nullptr)
		delete fsm_in.curr_session;

}


