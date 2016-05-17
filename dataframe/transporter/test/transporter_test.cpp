#include <gtest/gtest.h>
#include "transporter.hpp"
#include "logutils.hpp"
#include "test_structure.cpp"

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>
/**
 * TEST1:
 * Tests sending data of same length to all processes
 **/

TRTEST(TransportMultiDataHappyPath){
	Init("TransportMultiDataHappyPath");
	logger.ToggleDebug(true);
	logger.ToggleSTDOUT(false);
	transporter.logger.ToggleDebug(false);
	transporter.logger.ToggleSTDOUT(false);

	logger.LogTitle("Start");
	logger.Log(transporter.GetSummary());
	logger.Log("World size = " + std::to_string(world_size));
	logger.Log("INT_MAX = " + std::to_string(INT_MAX));
	logger.Log("LONG_MAX = " + std::to_string(LONG_MAX));

	logger.LogTitle("Starting, all processes check in!");

	transporter.Checkpoint();
	logger.Log(process_name + " ready!");	

	transporter.Checkpoint();

	sizev expected_size = 0;
	sizev* counts = (sizev*)malloc(sizeof(sizev) * world_size);
	std::vector<std::string> data(world_size);
//	std::string data[world_size];


	//Number of characters to send to each process
	sizev count = 1024;
	if(is_max)
		count = (INT_MAX / (world_size - 1 ) ); 
	//sizev count =  1024 * 1024; // 1 GB
	//If sending count elements, process should expect to recieve ( count * world_size ) elements



	logger.LogTitle("All processes, prepare data!");
	logger.Log("Reserving space");

	for(int i = 0; i < world_size; i++){
		if( rank == i )
			counts[i] = 0;
		else
			counts[i] = count;

		expected_size += counts[i];
		data[i].reserve(counts[i]);
	}

	logger.Log("Done reserving space");


	for(int i = 0; i < world_size; i++){

		logger.Log("Sending " + std::to_string(counts[i]) + " elements to process[" + std::to_string(i)+"]");
		for( sizev j = 0; j < counts[i]; j++){
			data[i].append(std::to_string(i));
		}
		logger.Log("Data for process[" + std::to_string(i) + "] prepared");
	}


	logger.LogTitle("All processes, open transports!");
	bool success = transporter.OpenTransport(counts);

	ASSERT_TRUE(success) << "Failed to open transport";

	logger.Log("Transport opened");
	transporter.Checkpoint();

	//Verify that the memory was allocated. If so, the first element is X (X marks the spot :) )	
	ASSERT_TRUE(transporter.window_base_addr_[0] == 'X') << "Window base address not initialized";
	logger.Log("Memory window is valid");

	logger.LogTitle("All processes, check windows!");

	//Verify size is correct
	sizev actual_size = transporter.GetWindowSize();
	ASSERT_EQ(expected_size,actual_size) << "Window size was not what was expected";
	logger.Log("Window size = " + std::to_string(actual_size));

	logger.LogTitle("All processes, send your data!");
	//Send stuff
	for(int i = rank; i <world_size; i++){
		char * c_data = new char[count];
		std::strcpy(c_data,data[i].c_str());
		logger.Log("Sending data to process["+std::to_string(i)+"]");
		transporter.Transport(
				c_data,
				data[i].length(),
				i
				);
		logger.Log("Sent data to process["+std::to_string(i)+"]");
	}

	for(int i = 0; i < rank; i++){
		char * c_data = new char[count];
		std::strcpy(c_data,data[i].c_str());
		logger.Log("Sending data to process["+std::to_string(i)+"]");

		transporter.Transport(
				c_data,
				data[i].length(),
				i
				);
		logger.Log("Sent data to process["+std::to_string(i)+"]");
	}


	//Now check that we recieved all the data
	
	transporter.Checkpoint();	
	logger.LogTitle("All processes, check your data!");
	for(sizev i = 0; i < transporter.GetWindowSize(); i++){
		EXPECT_EQ(transporter.window_base_addr_[i],rank + '0') 
			<< "For process "
			<< rank 
			<< " Expected rank, instead got "
			<< transporter.window_base_addr_[i];

	}
	logger.Log("All data recieved");

	//Note: As of current, transporter class does NOT free this memory
	free(transporter.window_base_addr_);
	free(counts);
	transporter.CloseTransport();

	logger.Log("Done");
}

/**
 * TEST2:
 * Tests sending large data to one other node
 **/
TRTEST(TransportSingleDataHappyPath){
	Init("TransportSingleDataHappyPath");
	
	sizev* counts;
	sizev count;
	sizev expected_window_size;
	std::string data[world_size];
	int dest;


		logger.ToggleDebug(true);
		logger.ToggleSTDOUT(false);
		transporter.logger.ToggleDebug(false);
		transporter.logger.ToggleSTDOUT(false);

	logger.LogTitle("Start");
		logger.Log(transporter.GetSummary());

	logger.LogTitle("Starting, all processes check in!");

		transporter.Checkpoint();
		logger.Log(process_name + " ready!");

		transporter.Checkpoint();

		expected_window_size = 0;
		counts = (sizev*)malloc(sizeof(sizev) * world_size);
		count =1024;
		if(is_max)
			count = INT_MAX;
		
		if ( rank == (world_size - 1))
			dest = 0;
		else
			dest = rank + 1;


	logger.LogTitle("All processes, prepare data!");
		logger.Log("Reserving space");
		for(int i = 0; i < world_size; i++){
			if ( i == dest )
				counts[i] = count;
			else
				counts[i] = 0;
		}

		expected_window_size = count;
		data[dest].reserve(count);
		logger.Log("Done reserving space");
	
	logger.LogTitle("All processes, generate data to send!");
		logger.Log("Generating data");
		for(int i = 0; i < count; i++){
			data[dest].append(std::to_string(dest));
		}
		logger.Log("Done generating data");

	logger.LogTitle("All processes, open transports!");
		bool success = transporter.OpenTransport(counts);
		ASSERT_TRUE(success) << "Failed to open transport";

		logger.Log("Transport opened");
		transporter.Checkpoint();

		CheckWindow();
		logger.Log("Memory window is valid");

		sizev actual_size = transporter.GetWindowSize();
		ASSERT_EQ(expected_window_size, actual_size)
			<< "Window size was not what was expected";
		logger.Log("Window size = " + std::to_string(actual_size));
		logger.Log("Transport ready!");

	logger.LogTitle("All processes send your data!");
		for(int i = 0; i < world_size; i++){
			char * c_data = new char[counts[i]];
			std::strcpy(c_data,data[i].c_str());
			logger.Log("Sending data to process["+std::to_string(i)+"]");
			transporter.Transport(
					c_data,
					data[i].length(),
					i
					);
			logger.Log("Sent data to process["+std::to_string(i)+"]");
		}

		logger.Log("All data sent");
		transporter.Checkpoint();

	logger.LogTitle("All processes, check your data!");
		for(sizev i = 0; i < transporter.GetWindowSize(); i++){
			EXPECT_EQ(transporter.window_base_addr_[i],rank+'0')
				<< "For process "
				<< rank
				<< "Expected rank, instead got "
				<< transporter.window_base_addr_[i];
		}
		logger.Log("All data recieved");

		free(transporter.window_base_addr_);

		transporter.CloseTransport();

		logger.Log("Done");
}



