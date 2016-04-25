#include "transporter.hpp"

#include <mpi.h>
#include <stdio.h>
#include <string>


/****************************************************************************
 *	Public Method Definitions - Constructors/Destructors
 ****************************************************************************/

Transporter::Transporter(){
	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &process_info_.world_size);  
	MPI_Comm_rank(MPI_COMM_WORLD, &process_info_.world_rank);
	char name[MPI_MAX_PROCESSOR_NAME];  
	int name_length;  
	MPI_Get_processor_name(name,&name_length); 
	process_info_.name = name;  
}

Transporter::~Transporter(){
	MPI_Finalize();
}

/****************************************************************************
 *	Public Method Defintions - Utils
 ****************************************************************************/
ProcessInfo Transporter::GetProcessInfo(){
	return process_info_;
}

std::string Transporter::GetSummary(){
	std::string summary = "Process Info: ";

	summary.append("\n\tProcess Name: ");
	summary.append(process_info_.name);

	summary.append("\n\tWorld Rank: ");
	summary.append(std::to_string(process_info_.world_rank));
	
	summary.append("\n\tWorld Size: ");
	summary.append(std::to_string(process_info_.world_size));

	return summary;
}


/****************************************************************************
 *	Public Method Definitions - MPI Operations
 ****************************************************************************/

void Transporter::Broadcast(
		char* send_data,
		char*& buffer,
		int data_size,
		int source){

	if(process_info_.world_rank == source){		
		buffer = send_data;
	} else{
		buffer = (char *)malloc(sizeof(char) * data_size);
	}

	MPI_Bcast(
			buffer,
			data_size,
			MPI_CHAR,
			source,
			MPI_COMM_WORLD
		);

}


void Transporter::Scatter(
		vector_str* send_data,
		char*& recv_data,
		int source
		){
	int data_size = process_info_.world_size;
	std::string buffer_str;
	int* send_count = new int[data_size];
	int* send_displ = new int[data_size];
	int send_size = 0;
	
	if(process_info_.world_rank == source){
	
		for(int i =0 ; i <data_size; i++){
			int size = send_data->at(i).size();
			buffer_str.append(send_data->data()[i]);
			send_count[i] = size;
			send_displ[i] = send_size;
			send_size+=size;
		}
	}


	//Let every process know how much data they should expect to recieve
	int recv_size = ScatterInt(
				send_count,
				source
			);

	recv_data = (char *)malloc(sizeof(char) * recv_size);

	char* buffer = new char[buffer_str.size()+1];
	buffer[buffer_str.size()] = 0;
	memcpy(buffer,buffer_str.c_str(),buffer_str.size());
	
	MPI_Scatterv(
			buffer,
			send_count,
			send_displ,
			MPI_CHAR,
			recv_data,
			recv_size,
			MPI_CHAR,
			source,
			MPI_COMM_WORLD
		    );
}


/****************************************************************************
 *	Private Method Definitions - MPI Operations
 ****************************************************************************/

int Transporter::ScatterInt(
		int* send_data,
		int source
		){
	int recv_int;

	MPI_Scatter(
			send_data,
			1,
			MPI_INT,
			&recv_int,
			1,
			MPI_INT,
			source,
			MPI_COMM_WORLD
		   );
//	std::string message = "recv_int = ";
//	message.append(std::to_string(recv_int));
//	Log(message);
	return recv_int;

}


/****************************************************************************
 *	Private Method Definitions - Utils
 ****************************************************************************/

void Transporter::Log(
		std::string message
		){
	std::cout << "Process[" << process_info_.world_rank << "] : " << message << std::endl;
}
