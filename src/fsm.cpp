#include "fsm.h"

//since the fsm has only two states we can use a boolean variable to represent them. 
//If variable is false then we're in IDLE state
static bool state = false;


void next_state(uint64_t payload){

  bool check_stop = payload == 0x66FF and state;
  bool check_start = (payload == 0x6601 or payload == 0xFF01) and !state;
    
  if(check_stop or check_start)
    state = not state;

}

void update_session(uint16_t id,uint64_t payload){

  static uint64_t session_id=0;
  static FILE* open_session=NULL;

  std::time_t timestamp = std::time(nullptr);
  std::string filename = "sessions/session_"+std::to_string(session_id);

  if(state){
    // if session file descriptor is null then it means that
    // the FSM was previously in IDLE state, then init a new session
    open_session = (open_session == NULL) ? fopen(filename.c_str(),"a") : open_session;
    fprintf(open_session,"%u %x#%x\n",timestamp,id,payload); 
  }else{
    if(open_session != NULL){
      fclose(open_session);
      open_session = NULL;
      session_id++; 
    }
  }
  
}
