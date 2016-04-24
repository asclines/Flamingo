#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include <string>

struct ProcessInfo{
	int world_size;
	int world_rank;
	std::string name;
};

class Transporter{
	public:
	 
//Constructors / Destructors 
		
		Transporter();
		~Transporter();

//Methods - Utils

		ProcessInfo GetProcessInfo();
		
		std::string GetSummary(); //For debugging use

//Methods - MPI Operations

	private:

//Data members

		ProcessInfo process_info_;

};


#endif
