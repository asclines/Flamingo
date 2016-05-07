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



int Transporter::GetWindowSize(){
	int flag;
	int *size;
	MPI_Win_get_attr(window_,MPI_WIN_SIZE,&size,&flag);
	return *size;

}

char * Transporter::GetWindowAddress(){
	return window_base_addr_;
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


bool Transporter::RequestWindow(int* counts, int*& recv_buffer){ //After program runs, counts holds the displ values

	recv_buffer = (int *)malloc(sizeof(int) * process_info_.world_size); //First used to hold recv_counts then to hold recv_displ
	int* send_displ = (int *)malloc(sizeof(int) * process_info_.world_size); //Values are size in bytes
	int total_recv_counts;
	int mpi_error_value = 0;	
	//Send number of elements wanting to send to each process
	mpi_error_value = MPI_Alltoall(
				counts,
				1,
				MPI_INT,
				recv_buffer,
				1,
				MPI_INT,
				MPI_COMM_WORLD
			);


	//Iterate through recv_counts to determine total size of memory needed and displacement
	send_displ[0] = 0;
	total_recv_counts = 0;
	for(int i = 0; i < process_info_.world_size-1; i++){
		int isize = recv_buffer[i] * sizeof(var);
		send_displ[i+1] = send_displ[i] + isize;
		total_recv_counts+= recv_buffer[i];
	}
	total_recv_counts+=recv_buffer[process_info_.world_size-1];

	//Send out displacement to each process
	mpi_error_value = MPI_Alltoall(
				send_displ,
				1,
				MPI_INT,
				recv_buffer,
				1,
				MPI_INT,
				MPI_COMM_WORLD
			    );

	//Create window
	int window_size = total_recv_counts * sizeof(var); 
//		Log("total_recv_counts = " +
//			std::to_string(total_recv_counts) +
//			" window_size = " + 
//			std::to_string(window_size)
//		);

	MPI_Alloc_mem(window_size,MPI_INFO_NULL, &window_base_addr_);
	window_base_addr_[0] = 'X';
	mpi_error_value = MPI_Win_create(
				window_base_addr_,
				window_size,
				sizeof(var),
				MPI_INFO_NULL,
				MPI_COMM_WORLD,
				&window_
			      );
//	MPI_Win_fence(0,window_);	

	MPI_Comm_group(
		MPI_COMM_WORLD,
		&group_
		);

//	MPI_Win_start(group_,0,window_);

///	MPI_Win_fence(0,window_);

	*counts = *recv_buffer;
//	MPI_Barrier(MPI_COMM_WORLD);
	
//	MPI_Win_post(group_,0,window_);
	return(mpi_error_value == 0);	
}

void Transporter::CloseWindow(){
//	MPI_Win_complete(window_);
//	MPI_Win_fence(0,window_);
	MPI_Win_free(&window_);
//	Log("Window Closed");
}

void Transporter::Send(
			var *data,
			int size,
			int dest,
			int displ){

	int mpi_error_value = 0;
//	MPI_Win_start(group_,0,window_);
//	Log("Send: size = " + std::to_string(size) + " dest = " + std::to_string(dest) + " displ = " + std::to_string(displ));

	MPI_Win_lock(MPI_LOCK_EXCLUSIVE,dest,0,window_);
	mpi_error_value = MPI_Put(
				data,
				size,
				MPI_CHAR,
				dest,
				displ,
				size,
				MPI_CHAR,
				window_
			);
	MPI_Win_unlock(dest,window_);
	
//	Log("mpi_error_value = " + std::to_string(mpi_error_value));
//	MPI_Win_complete(window_);
	MPI_Win_fence(0,window_);

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
	return recv_int;

}

void Transporter::BroadcastInt(
		int* value,
		int source){

	MPI_Bcast(
			value,
			1,
			MPI_INT,
			source,
			MPI_COMM_WORLD
		);
}
/****************************************************************************
 *	Private Method Definitions - Utils
 ****************************************************************************/

void Transporter::Log(
		std::string message
		){
	std::cout << "Process[" << process_info_.world_rank << "] : " << message << std::endl;
}
