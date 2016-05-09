#include <gtest/gtest.h>
#include "transporter.hpp"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>

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
	std::string test_name;

	void Init(std::string name){
		test_name = name;
		world_size = transporter.GetProcessInfo().world_size;
		rank = transporter.GetProcessInfo().world_rank;
	}

	void LogTitle(std::string message){
		transporter.Checkpoint();

		if(transporter.GetProcessInfo().world_rank == 0){
			std::ofstream output_file ("output_test.out", std::ios::out| std::ios::app);
	
			std::string out = "\n<----------";
			out.append(message);
			out.append("---------->\n");
	
			if(output_file.is_open()){
				output_file << out;
				output_file.close();
			}
		}

		transporter.Checkpoint();
	}

	void Log(std::string message){
		time_t rawtime;
		struct tm * timeinfo;
		time (&rawtime);
		timeinfo = localtime(&rawtime);

		std::string out = "[";
		out.append(asctime(timeinfo));
		out.pop_back();
		out.append("] Process[");
		out.append(std::to_string(transporter.GetProcessInfo().world_rank));
		out.append("]:");	       
		out.append(message);
		out.append("\n");


		std::ofstream output_file ("output_test.out", std::ios::out| std::ios::app);
			
		while(!output_file.is_open()){
		}

		output_file << out;
		output_file.close();
	}	

	void LogOnce(std::string message){
		int node = 0;

		if(node == rank){
			Log(message);
		}
	}
};

/**
 * TEST 1:
 * Tests sending data of same length to all processes
 **/

TRTEST(Test1){
	Init("Test1");
	LogTitle("Starting, all processes check in!");
	Log("Ready!");	

	int* counts = (int *)malloc(sizeof(int) * world_size);
	std::string data[world_size];
	int count = 50000; //Number of elements to send to each node


	LogTitle("All processes, prepare data!");
	for(int i = 0; i < world_size; i++){
		counts[i] = count;

		for( int j = 0; j < counts[i]; j++){
			data[i].append(std::to_string(i));
		}
	}


	LogTitle("All processes, open transports!");
	bool success = transporter.OpenTransport(counts);

	ASSERT_TRUE(success);

	transporter.Checkpoint();

	//Verify that the memory was allocated. If so, the first element is X (X marks the spot :) )	
	ASSERT_TRUE(transporter.window_base_addr_[0] == 'X');

	LogTitle("All processes, check windows!");

	//Verify size is correct
	int expected_size = count * world_size;
	int actual_size = transporter.GetWindowSize();
	ASSERT_EQ(expected_size,actual_size);

	LogTitle("All processes, send your data!");
	//Send stuff
	for(int i = 0; i <world_size; i++){
		char * c_data = new char[count];
		std::strcpy(c_data,data[i].c_str());
		transporter.Transport(
				c_data,
				data[i].length(),
				i
				);

	}

	//Now check that we recieved all the data
	
	transporter.Checkpoint();	
	LogTitle("All processes, check your data!");
	for(int i = 0; i < transporter.GetWindowSize(); i++){
		EXPECT_EQ(transporter.window_base_addr_[i],rank + '0') 
			<< "For process "
			<< rank 
			<< " Expected rank, instead got "
			<< transporter.window_base_addr_[i];

	}


	transporter.CloseTransport();

}
