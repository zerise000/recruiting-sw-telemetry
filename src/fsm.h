#ifndef __FSM_H__
#define __FSM_H__

#include <iostream>
#include <cstdint>
#include <queue>

#include "session.h"
#include "message.h"

#define UPDATE(fsm_in) \
			(fsm_in->curr_session)->update_session(msg_in);\	
	  		fsm_in->state = not fsm_in->state; 
		
typedef struct {
	Session* curr_session;

	//since the fsm has only two states we can use a boolean variable to represent them. 
	//If variable is false then we're in IDLE state
	bool state = false;
} Fsm;

void next_state(Fsm* fsm_in,Message msg_in);
void save_msg(std::queue<Message> &msg_ch); 

#endif
