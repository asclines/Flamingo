#include <gtest/gtest.h>
#include "transporter.hpp"
#include "logutils.hpp"
#include "test_structure.cpp"

#include <string>

/**
 * Tests every process sending a broadcast,
 * the message will be [element_count]  elements each element being the value of the sending process id
 **/
TRTEST(SimpleBroadcastHappyPath){
	Init("SimpleBroadcastHappyPath");
	logger.ToggleDebug(true);
	logger.ToggleSTDOUT(false);
	transporter.logger.ToggleDebug(false);
	transporter.logger.ToggleSTDOUT(false);

	logger.LogTitle("Start");

	logger.LogTitle("Starting");
	transporter.Checkpoint();
	logger.Log(process_name + " ready!");

	int element_count = 1024;
	if(is_max)
		element_count = INT_MAX/2;

	logger.Log("Element count = " + std::to_string(element_count));
	char *send_data = (char *)malloc(sizeof(char) * element_count);
	char *recv_data;



	for(int source_node = 0; source_node < world_size; source_node++){
		if(rank == source_node){
			logger.Log("Prepping data to broadcast!");
			for(int i =0; i < element_count; i++){
				send_data[i] = source_node;
			}
			logger.Log("Data prepared!");
		}
		transporter.Checkpoint();
		logger.Log("Broadcasting from process["+std::to_string(source_node) + "]");

		transporter.Broadcast(
				send_data,
				recv_data,
				element_count,
				source_node
				);

		logger.Log("Done broadcasting");
		transporter.Checkpoint();
		logger.LogTitle("All processes except source node! Check recieved data!");
		if(rank != source_node){
			for(int i = 0; i < element_count; i++){
				int actual_value = recv_data[i];// + '0';
				ASSERT_EQ(source_node,actual_value) 
					<< "Expected value from broadcast was "
					<< source_node
					<< " actual value is "
					<< actual_value;
			}
		}
		logger.Log("Done checking recieved data!");

	}
	
	logger.Log("Done");	
}


TRTEST(KnownSizeIteratorBroadcastHappyPath){
	Init("KnownSizeIteratorBroadcastHappyPath");
	logger.ToggleDebug(true);
	logger.ToggleSTDOUT(false);
	transporter.logger.ToggleDebug(false);
	transporter.logger.ToggleSTDOUT(false);

	logger.LogTitle("Start");

	logger.LogTitle("Starting");
	transporter.Checkpoint();
	logger.Log(process_name + " ready!");

	int element_count = 1024;
	if(is_max)
		element_count = INT_MAX/2;
	logger.Log("Element count = " + std::to_string(element_count));

	std::vector<char> send_data(element_count);
	char *recv_data;

	for (int source_node = 0; source_node < world_size; source_node++){
		if(rank == source_node){
			logger.Log("Prepping data to broadcast!");
			for(int i = 0; i < element_count; i++){
				send_data[i] = source_node;
			}
			logger.Log("Data prepared!");
		}
		transporter.Checkpoint();
		logger.Log("Broadcasting from process["+std::to_string(source_node) + "]");

		transporter.Broadcast<std::vector<char>::iterator>(
				send_data.begin(),
				send_data.end(),
				element_count,
				recv_data,
				source_node
				);
		logger.Log("Done broadcasting");
		transporter.Checkpoint();
		logger.LogTitle("All processes except source node! Check recieved data!");
		if(rank != source_node){
			for(int i =0 ; i < element_count; i++){
				int actual_value = recv_data[i];
				ASSERT_EQ(source_node,actual_value)
					<< "Expected value from broadcast was "
					<< source_node
					<< " actual value is "
					<< actual_value;
			}
		}
		logger.Log("Done checking recieved data!");
	}

	logger.Log("Done");
}

TRTEST(UknownSizeIteratorBroadcastHappyPath){
	Init("UknownSizeIteratorBroadcastHappyPath");
	logger.ToggleDebug(true);
	logger.ToggleSTDOUT(false);
	transporter.logger.ToggleDebug(false);
	transporter.logger.ToggleSTDOUT(false);

	logger.LogTitle("Start");

	logger.LogTitle("Starting");
	transporter.Checkpoint();
	logger.Log(process_name + " ready!");

	int element_count = 1024;
	if(is_max)
		element_count = INT_MAX/2;
	logger.Log("Element count = " + std::to_string(element_count));

	std::vector<char> send_data(element_count);
	char *recv_data;

	for (int source_node = 0; source_node < world_size; source_node++){
		if(rank == source_node){
			logger.Log("Prepping data to broadcast!");
			for(int i = 0; i < element_count; i++){
				send_data[i] = source_node;
			}
			logger.Log("Data prepared!");
		}
		transporter.Checkpoint();
		logger.Log("Broadcasting from process["+std::to_string(source_node) + "]");

		transporter.Broadcast<std::vector<char>::iterator>(
				send_data.begin(),
				send_data.end(),
				recv_data,
				source_node
				);
		logger.Log("Done broadcasting");
		transporter.Checkpoint();
		logger.LogTitle("All processes except source node! Check recieved data!");
		if(rank != source_node){
			for(int i =0 ; i < element_count; i++){
				int actual_value = recv_data[i];
				ASSERT_EQ(source_node,actual_value)
					<< "Expected value from broadcast was "
					<< source_node
					<< " actual value is "
					<< actual_value;
			}
		}
		logger.Log("Done checking recieved data!");
	}

	logger.Log("Done");
}

