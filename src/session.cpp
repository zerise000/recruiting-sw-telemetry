#include "session.h"


Session::Session(){
	// generate new ID
	duration_t now = std::chrono::system_clock::now();
	uint64_t millisec_diff = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	this->id += (1+(rand() % 10)+millisec_diff);

	// open log file
  	std::string log_filename = "logs/session_"+std::to_string(this->id)+".log";
	this->log_file.open(log_filename, std::ios_base::app);

	// reset freq and diff_sum arrays
	memset(this->freq,0,max_id_size);
	memset(this->diff_sum,0,max_id_size);

	// memset is not able to use std::chrono, so we need to set the array manually
	duration_t init_time = std::chrono::system_clock::now();

	for(size_t i=0; i<max_id_size; i++)
		this->last_msg_time[i] = init_time;
}

Session::~Session(){

	// if file has already been closed then exit without doing nothing
	if(not this->log_file.is_open())
		return ;

	// close the log file
	this->log_file.close();

	// save statistics into CSV file
	this->compute_statistics();	
}

void Session::compute_statistics(){
	// init necessary variables
	
	std::string csv_filename = "csv/statistics_session_"+std::to_string(this->id)+".csv";
	std::ofstream csv_statistics;
	csv_statistics.open(csv_filename, std::ios_base::app);

	// put header into file
	csv_statistics << "Id;number_of_messages;mean_time\n";

	double mean = 0;
	for(size_t i=0; i<max_id_size; i++){

		// if frequency of i-th position is zero then it means that there hasn't been messages
		// with ID equal to index i. In this case don't do anything, otherwise compute the mean
		// and save datas in the CSV file
		if(this->freq[i] != 0){
			mean = (this->diff_sum[i]/this->freq[i]); 
			csv_statistics << std::format("{:x};{};{:f}\n",i,this->freq[i],mean);
		}
	}

	// close csv file
	csv_statistics.close();
}

void Session::update_session(Message msg){

	// we get the current timestamp and compute the difference time in millisecond 
	duration_t now = std::chrono::system_clock::now();
	uint64_t millisec_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_msg_time[msg.id]).count();
	uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

	// save operation in log file, then compute current fields
	this->log_file << timestamp << " " << std::format("{:x}#{:x}\n",msg.id,msg.payload);  
	this->freq[msg.id] += 1;
	this->diff_sum[msg.id] += (double)millisec_diff;

	this->last_msg_time[msg.id] = now;

}
