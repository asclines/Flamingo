#include <gtest/gtest.h>
#include "transporter.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#define TRTEST(name)\
	TEST_F(TransporterTest,name)

class TransporterTest : public ::testing::Test{

protected:
	
	static void SetUpTestCase(){
		
	}

	virtual void SetUp(){

	}


	Transporter transporter;


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
	LogOnce("Starting, all processes check in!");
	Log("Ready!");	
}
