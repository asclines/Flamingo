#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include <string>
#include <vector>
#include <string>

struct ProcessInfo{
	int world_size;
	int world_rank;
	std::string name;
};

class Transporter{
	public:
//Typedefs
	typedef std::vector<std::string> vector_str;
	typedef std::vector<std::string>::iterator iterator_str;
		
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
		
		/*
		 * Broadcast method where data_size is known by all processes
		 */
		template<typename iteratorT>
		void Broadcast(
				iteratorT begin,
				iteratorT end,
				int data_size,
				char*& data_recv,
				int source
			      );

		/*
		 * Scatter method where the receiving processes don't know the data_size 
		 */
		void Scatter(
				vector_str* send_data,
				char*& recv_data,
				int source
			    );
	private:
//Methods - MPI Operations

		/*
		 * Scatter method used to send an array of ints to processess, 1 int per process
		 */
		int ScatterInt(
				int* send_data, //Array of ints with size == number of processes
				int source
				);		
//Methods - Utils
		void Log(std::string message);
//Data members

		ProcessInfo process_info_;

};

#include "transporter.inl"
#endif
