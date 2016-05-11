#include <gtest/gtest.h>
#include "transporter.hpp"
#include "logutils.hpp"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>

#define TRTEST(name)\
	TEST_F(TransporterTest,name)

class TransporterTest : public ::testing::Test{

protected:

//	TransporterTest() : transporter(false){}	
	static void SetUpTestCase(){
		
	}

	virtual void SetUp(){

	}

	virtual void TearDown(){

	}

	Transporter transporter;
	int world_size;
	int rank;
	std::string process_name;
	std::string test_name;

	void Init(std::string name){
		test_name = name;
		world_size = transporter.GetProcessInfo().world_size;
		rank = transporter.GetProcessInfo().world_rank;
		process_name = transporter.GetProcessInfo().name;
	}

};

/**
 * TEST 1:
 * Tests sending data of same length to all processes
 **/

TRTEST(Test1){
	Init("Test1");
	TransportLogger logger(transporter.GetProcessInfo());
	logger.SetLogToFile(std::to_string(rank) + "-Test1");
	logger.LogTitle("Start");
	
	logger.Log("World size = " + std::to_string(world_size));
	logger.Log("INT_MAX = " + std::to_string(INT_MAX));

	logger.LogTitle("Starting, all processes check in!");

	transporter.Checkpoint();
	logger.Log(process_name + " ready!");	

	transporter.Checkpoint();


	int* counts = (int *)malloc(sizeof(int) * world_size);
	std::string data[world_size];

	// Note: count cannot be >= (INT_MAX / world_size) -1 TODO: <- confirm that
	int count = 500 ;//(INT_MAX / ( 2 * world_size)); //Number of elements to send to each node


	logger.LogTitle("All processes, prepare data!");
	for(int i = 0; i < world_size; i++){
		counts[i] = count;

		logger.Log("Sending " + std::to_string(counts[i]) + " elements to process[" + std::to_string(i)+"]");
		for( int j = 0; j < counts[i]; j++){
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
	int expected_size = count * world_size;
	int actual_size = transporter.GetWindowSize();
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
	for(int i = 0; i < transporter.GetWindowSize(); i++){
		EXPECT_EQ(transporter.window_base_addr_[i],rank + '0') 
			<< "For process "
			<< rank 
			<< " Expected rank, instead got "
			<< transporter.window_base_addr_[i];

	}
	logger.Log("All data recieved");

	//Note: As of current, transporter class does NOT free this memory
	free(transporter.window_base_addr_);
	transporter.CloseTransport();

	logger.Log("Done");

}
