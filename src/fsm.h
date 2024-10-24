#ifndef __FSM_H__
#define __FSM_H__

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <ctime>
#include <cstring>

void next_state(uint64_t payload);
void update_session(uint16_t id,uint64_t payload);

#endif
