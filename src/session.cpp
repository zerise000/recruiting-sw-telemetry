#include "session.h"

void Session::start_new_session(){

  	std::string log_filename = "logs/session_"+std::to_string(this->id)+".log";
	this->log_file.open(log_filename, std::ios_base::app);

	memset(this->freq,0,max_id_size);
	memset(this->diff_sum,0,max_id_size);

	for(size_t i=0; i<max_id_size; i++)
		this->last_msg_time[i] = std::chrono::system_clock::now();
}

void Session::stop_session(){

	if(not this->log_file.is_open())
		return ;

	this->log_file.close();

	std::string csv_filename = "csv/statistics_session_"+std::to_string(this->id)+".csv";
	std::ofstream csv_statistics;
	csv_statistics.open(csv_filename, std::ios_base::app);

	this->id++;
	csv_statistics << "Id;number_of_messages;mean_time\n";

	double mean = 0;
	for(size_t i=0; i<max_id_size; i++){

		if(this->freq[i] != 0){
			mean = (this->diff_sum[i]/this->freq[i]); 
			csv_statistics << std::format("{:x};{};{:f}\n",i,this->freq[i],mean);
		}
	}

	csv_statistics.close();
	
}

void Session::update_session(Message msg){

	duration_t now = std::chrono::system_clock::now();
	uint64_t millisec_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_msg_time[msg.id]).count();
	uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

	this->log_file << timestamp << " " << std::format("{:x}#{:x}\n",msg.id,msg.payload);  
	this->freq[msg.id] += 1;
	this->diff_sum[msg.id] += (double)millisec_diff;

	this->last_msg_time[msg.id] = now;

}
