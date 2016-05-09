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

	TransporterTest() : transporter(false){}	
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

	void LogOnce(std::string message){
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
};



TRTEST(Test1){
	Init("Test1");
	LogOnce("Starting, all processes check in!");
	Log("Ready!");	

	int* counts = (int *)malloc(sizeof(int) * world_size);
	std::string data[world_size];
	int count = 5; //Number of elements to send to each node

	for(int i = 0; i < world_size; i++){
		counts[i] = count;

		for( int j = 0; j < counts[i]; j++){
			data[i].append(std::to_string(i));
		}
	}


		bool success = transporter.OpenTransport(counts);

		ASSERT_TRUE(success);

		transporter.Checkpoint();

		//Verify that the memory was allocated. If so, the first element is X (X marks the spot :) )
		char * addr;
		transporter.GetWindowAddress(&*addr);
		
		Log("Window[0] = " + std::to_string(addr[0]));
		
		ASSERT_TRUE(addr[0] == 'X');
		int expected_size = count * world_size;
		int actual_size = transporter.GetWindowSize();
		ASSERT_EQ(expected_size,actual_size);

		//Send stuff
		for(int i = 0; i <world_size; i++){
			char * c_data = new char[world_size];

			std::strcpy(c_data,data[i].c_str());

			transporter.Transport(
					c_data,
					data[i].length(),
					i
					);

		}

		//Now check that we recieved all the data
	
		transporter.Checkpoint();	
		std::string message = "After send, Window = ";
		for(int i = 0; i < transporter.GetWindowSize(); i++){
			message.push_back(addr[i]);

		//	EXPECT_TRUE(addr[i] == rank) 
		//		<< "For process "
		//		<< rank 
		//		<< " Expected rank, instead got "
		//		<< addr[i];

		}

		Log(message);

		transporter.CloseTransport();

}
