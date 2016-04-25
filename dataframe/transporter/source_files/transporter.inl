#include "transporter.hpp"

#include <vector>
#include <iostream>


template<typename iteratorT>
void Transporter::Broadcast(
		iteratorT begin,
		iteratorT end,
		int data_size,	
		char*& data_recv,
		int source
		){

	iteratorT current;
	std::vector<char> buffer;
	
	if(process_info_.world_rank == source){

		for(current = begin; current!=end; current++){
			buffer.push_back(*current);
		}
	}
	Broadcast(
			buffer.data(),
			data_recv,
			data_size,
			source
		 );

}


template<typename iteratorT>
void Transporter::Broadcast(
		iteratorT begin,
		iteratorT end,
		char*& data_recv,
		int source){
	iteratorT current;
	std::vector<char> buffer;
	int data_size = 0;

	if(process_info_.world_rank == source){
		for(current = begin; current!= end; current++){
			data_size++;
			buffer.push_back(*current);			
		}
	}

	//Let everyone know what size the data is
	BroadcastInt(&data_size,source);

	Broadcast(
			buffer.data(),
			data_recv,
			data_size,
			source
		 );

}
