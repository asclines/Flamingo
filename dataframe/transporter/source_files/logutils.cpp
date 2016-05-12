#include "logutils.hpp"
#include "transporter.hpp"


TransportLogger::TransportLogger(){
	debug_on_ = false;
}

TransportLogger::~TransportLogger(){
	if (log_file_stream_.is_open() ){
		log_file_stream_.close();
	}
}



std::string TransportLogger::GetPrettyProcess(){

	std::string name = "Process[" + 
		std::to_string(process_rank)
		+ "]";

	return name;

}


std::string TransportLogger::GetTimeStamp(){
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	std::string timestamp = asctime(timeinfo);
	timestamp.pop_back();

	return timestamp;
}

void TransportLogger::SetLogToFile(std::string log_file_name){
	log_to_file_ = true;
	log_file_name_ = "out/" + log_file_name + ".out";
	log_file_stream_.open(log_file_name_,std::ios::out|std::ios::app);

}


void TransportLogger::ToggleDebug(bool debug_on){
	debug_on_ = debug_on;
}

void TransportLogger::ToggleSTDOUT(bool stdout_on){
	stdout_on_ = stdout_on;
}

void TransportLogger::ToggleLogFile(bool log_to_file){
	log_to_file_ = log_to_file;
}

void TransportLogger::SetProcessInfo(int size,int rank, std::string name){
	world_size = size;
	process_rank = rank;
	process_name = name;
}

void TransportLogger::LogTitle(std::string message){
	if(!debug_on_)
		return;

	std::string out = "\n<----------" 
		+ message
		+ "---------->\n";

	Output(out, true);
}

void TransportLogger::Log(std::string message){
	if(!debug_on_)
		return;

	std::string log_message = GetTimeStamp() 
		+ ":" 
		+ GetPrettyProcess() 
		+ ":"
		+ message
		+ "\n";

	Output(log_message);

}



void TransportLogger::Output(std::string log_message, bool stdout_once){
	if(!debug_on_)
		return;

	if(stdout_on_)
		if((stdout_once && process_rank == 0) || !stdout_once )
			std::cout << '\t' << log_message;
			

	if(log_to_file_){
		if(!log_file_stream_.is_open())
			log_file_stream_.open(log_file_name_,std::ios::out|std::ios::app);


		if(log_file_stream_.is_open())
			log_file_stream_<< log_message;

	}

}
