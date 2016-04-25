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
		void Broadcast(
				char* send_data, //Null if not source
				char*& recv_data, //Empty pointer
				int data_size, // Number of elements in send_data
				int source //Node ID of process sending data
			      );

		template<typename iteratorT>
		void Broadcast(
				iteratorT begin,
				iteratorT end,
				int data_size,
				char*& data_recv,
				int source
			      );

	private:

//Data members

		ProcessInfo process_info_;

};

#include "transporter.inl"
#endif
