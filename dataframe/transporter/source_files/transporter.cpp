#include "transporter.hpp"

#include <mpi.h>
#include <stdio.h>
#include <string>


/****************************************************************************
 *	Public Method Definitions
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


