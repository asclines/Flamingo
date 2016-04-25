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
	
	for(current = begin; current!=end; current++){
		buffer.push_back(*current);
	}

	Broadcast(
			buffer.data(),
			data_recv,
			data_size,
			source
		 );

}
