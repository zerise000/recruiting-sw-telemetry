#ifndef __FSM_H__
#define __FSM_H__

#include <iostream>
#include <cstdint>
#include <queue>

#include "session.h"
#include "message.h"

void next_state(Session* curr_session,Message msg_in);
void save_msg(std::queue<Message> &msg_ch); 

#endif
