#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wliteral-suffix"
#include <mpi.h>
#include <string>
#include <vector>
#include <string>

#pragma GCC diagnostic pop

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
	typedef char var; //Unit type for data being send
	typedef char* varptr;

//Constructors / Destructors 
		
		Transporter();
		~Transporter();

//Methods - Utils

		void Log(std::string message);

		ProcessInfo GetProcessInfo();
		
		std::string GetSummary(); //For debugging use

		int GetWindowSize(); //Returns flag status

		char * GetWindowAddress(); //Return pointer to base address of window

//Methods - MPI Operations
		
		/*
		 * Broadcast method where data_size is known by all processes
		 */
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
		 * Broadcast method where data_size is NOT known by all processes
		 */
		template<typename iteratorT>
		void Broadcast(
				iteratorT begin,
				iteratorT end,
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

		bool RequestWindow(
				int* counts, 
				int*& recv_displ
				);

		void CloseWindow();

		void Send(
				char* data,
				int size,
				int dest,
				int displ
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

		/*
		 * Broadcast method used to send a single int to processes
		 */		
		void BroadcastInt(
				int* value, //The value being broadcasted
				int source
				);

//Methods - Utils
//Data members
		var *window_base_addr_;	
		ProcessInfo process_info_;
		MPI_Win window_;
		MPI_Group group_; //Group of all process

};

#include "transporter.inl"
#include "transporter.cpp"
#endif
