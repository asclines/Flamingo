#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#define MPI_SIZEV MPI_LONG

#include "logutils.hpp"

#include <mpi.h>
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
	typedef char var; //Unit type for data being send
	typedef long int sizev; //Value used for window size, NOTE! MPI_SIZEV must be defined the same
//	typedef MPI_LONG MPI_SIZEV; //MPI equivalent of whatever the above line is

	var *window_base_addr_;	
	TransportLogger logger;
//Constructors / Destructors 
		
	Transporter();
	~Transporter();

//Methods - Utils


	ProcessInfo GetProcessInfo();
	
	std::string GetSummary(); //For debugging use
	
	sizev GetWindowSize(); //Returns flag status


	void Checkpoint(); //Blocks processes until this point has been reached

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

	bool OpenTransport(sizev* counts);

	void CloseTransport();

	bool Transport(
			char* data,
			sizev size,
			int dest
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
	sizev *displ; //array of size equal to world size where value is the displacement in the window at the node with value of index in this array
	ProcessInfo process_info_;
	MPI_Win window_;
	MPI_Group group_; //Group of all process
		

};

#include "transporter.inl"
#include "transporter.cpp"
#endif
