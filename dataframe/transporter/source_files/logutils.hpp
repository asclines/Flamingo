#ifndef TRANSPORTER_LOGUTILS_H
#define TRANSPORTER_LOGUTILS_H

#include "transporter.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

class TransportLogger{
public:

	TransportLogger();
	~TransportLogger();

//Helper methods
	std::string GetPrettyProcess(); //For process id 1, returns Process[1]

	std::string GetTimeStamp();

	void SetLogToFile(std::string log_file_name); 

	void ToggleSTDOUT(bool stdout_on);

	void ToggleDebug(bool);

	void ToggleLogFile(bool);

	void SetProcessInfo(
			int world_size,
			int world_rank,
			std::string process_name
			);

//Actual log methods
	void LogTitle(std::string message);

	void Log(std::string message);

private:

	void Output(
			std::string message,
			bool stdout_once = false//If logging to STDOUT, only log once ( intended used is for LogTitle) uses Process[0] to log
			);

	
	//Flags
	bool debug_on_;
	bool log_to_file_;
	bool stdout_on_;

	//Log file 
	std::string log_file_name_;
	std::ofstream log_file_stream_;

	int process_rank;
	int world_size;
	std::string process_name;


};

#include "logutils.cpp"
#endif
